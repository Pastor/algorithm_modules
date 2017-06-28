#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <string>
#include <iostream>
#include <fstream>
#include <thread>

#if defined(_DEBUG)
#undef _DEBUG

#include <Python.h>

#define _DEBUG
#else
#include <Python.h>
#endif

#include <python_plugin_module.h>

static bool start_plugin(PyObject *fun, std::shared_ptr<ModuleContext> context) {
    auto arg_context = PyDict_New();
    if (context) {
        const auto &prop = context->entries();
        for (auto it = prop.begin(); it != prop.end(); ++it) {
            auto value = PyString_FromFormat((*it).second.c_str());
            PyDict_SetItemString(arg_context, (*it).first.c_str(), value);
            Py_DecRef(value);
        }
    }
    auto arg_name = PyString_FromFormat("PythonScriptModule");
    if (fun && PyCallable_Check(fun)) {
        /**TODO: https://docs.python.org/3.6/c-api/arg.html#c.Py_BuildValue */
        auto args = Py_BuildValue("OO", arg_context, arg_name);
        Py_DecRef(arg_name);
        Py_DecRef(arg_context);
        auto result = PyEval_CallObject(fun, args);
        fprintf(stdout, "Result: %d\n", _PyInt_AsInt(result));
        Py_DecRef(args);
        Py_DecRef(result);
        return true;
    } else {
        Py_DecRef(arg_name);
        Py_DecRef(arg_context);
    }
    return false;
}

static bool execute(const std::string content, std::shared_ptr<ModuleContext> context) {
    auto compiled_fun = Py_CompileString(content.c_str(), "", Py_file_input);
    auto compiled_module = PyImport_ExecCodeModule(const_cast<char *>(std::string("plugin").c_str()), compiled_fun);
    {
        auto os_module = PyImport_ImportModule("os");
        PyModule_AddObject(compiled_module, "os", os_module);
        Py_DecRef(os_module);
    }
    auto fun = PyObject_GetAttrString(compiled_module, "main");
    auto ret = start_plugin(fun, context);
    Py_DecRef(fun);
    Py_DecRef(compiled_module);
    Py_DecRef(compiled_fun);
    return ret;
}

struct PythonScriptModulePrivate final {
    PythonScriptModulePrivate(const std::string &content) : _content(content) {}

    bool execute(std::shared_ptr<ModuleContext> context) {
        bool ret;
        //Py_BEGIN_ALLOW_THREADS
        auto state = PyGILState_Ensure();
        ret = ::execute(_content, context);
        PyGILState_Release(state);
        //Py_END_ALLOW_THREADS
        return ret;
    }

private:
    const std::string _content;
};

PythonScriptModule::PythonScriptModule(const std::string &content,
                                       const std::string &name,
                                       const std::string &description,
                                       double version)
        : ScriptModule(name, description, version), d(new PythonScriptModulePrivate(content)) {
    if (!Py_IsInitialized()) {
        Py_Initialize();
        PyEval_InitThreads();
    }
}

PythonScriptModule::~PythonScriptModule() {
    Py_Finalize();
}

bool
PythonScriptModule::execute(std::shared_ptr<ModuleContext> context) {
    return d->execute(context);
}

PythonFileScriptModule::PythonFileScriptModule(const std::string &file_path,
                                               const std::string &name,
                                               const std::string &description,
                                               double version)
        : ScriptModule(name, description, version, file_path) {
    std::ifstream script(file_path, std::ios_base::in);
    if (script.is_open()) {
        auto content = std::string(std::istreambuf_iterator<char>(script), std::istreambuf_iterator<char>());
        d = std::shared_ptr<PythonScriptModule>(new PythonScriptModule(content, name, description, version));
        script.close();
    } else {
        fprintf(stderr, "[PythonFileScriptModule] Script %s can't read, error: %s\n", file_path.c_str(), strerror(errno));
    }
}

PythonFileScriptModule::~PythonFileScriptModule() {

}

bool
PythonFileScriptModule::execute(std::shared_ptr<ModuleContext> context) {
    return d ? d->execute(context) : false;
}
