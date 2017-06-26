#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <thread>
#include <Python.h>

#include <python_plugin_module.h>

static bool start_plugin(PyObject *fun, std::shared_ptr<ModuleContext> context) {
    auto arg_context = PyString_FromFormat("context");
    auto arg_connection = PyString_FromFormat("connection");
    if (fun && PyCallable_Check(fun) && *context) {
        /**TODO: https://docs.python.org/3.6/c-api/arg.html#c.Py_BuildValue */
        auto args = Py_BuildValue("OO", arg_context, arg_connection);
        auto result = PyEval_CallObject(fun, args);
        fprintf(stdout, "Result: %d\n", _PyInt_AsInt(result));
        Py_DecRef(args);
        return true;
    }
    return false;
}

static bool execute(const std::string content, std::shared_ptr<ModuleContext> context) {
    auto compiled_fun = Py_CompileString(content.c_str(), "", Py_file_input);
    auto compiled_module = PyImport_ExecCodeModule(const_cast<char *>(std::string("plugin").c_str()), compiled_fun);
    {
        auto hashlib_module = PyImport_ImportModule("os");
        PyModule_AddObject(compiled_module, "os", hashlib_module);
        //Py_DecRef(hashlib_module);
    }
    auto fun = PyObject_GetAttrString(compiled_module, "main");
    auto ret = start_plugin(fun, context);
    Py_DecRef(fun);
    Py_DecRef(compiled_module);
    Py_DecRef(compiled_fun);
    return ret;
}

struct PythonScriptModulePrivate final {
    struct Lock {
        Lock() {
            PyEval_AcquireLock();
        }

        ~Lock() {
            PyEval_ReleaseLock();
        }
    };

    struct RestoreState {
        RestoreState() {
            _ts = PyThreadState_Get();
        }

        ~RestoreState() {
            PyThreadState_Swap(_ts);
        }

    private:
        PyThreadState *_ts;
    };

    struct SwapState {
        SwapState(PyThreadState *ts) {
            _ts = PyThreadState_Swap(ts);
        }

        ~SwapState() {
            PyThreadState_Swap(_ts);
        }

    private:
        PyThreadState *_ts;
    };

    struct ThreadState {
        ThreadState(PyInterpreterState *interp) {
            _ts = PyThreadState_New(interp);
        }

        ~ThreadState() {
            if (_ts) {
                PyThreadState_Clear(_ts);
                PyThreadState_Delete(_ts);

                _ts = nullptr;
            }
        }

        operator PyThreadState *() {
            return _ts;
        }

    private:
        PyThreadState *_ts;
    };

    struct Interpreter {
        struct Thread {
            Lock _lock;
            ThreadState _state;
            SwapState _swap;

            Thread(PyInterpreterState *interp) : _state(interp), _swap(_state) {}
        };

        Interpreter() {
            RestoreState restore;

            _ts = Py_NewInterpreter();
        }

        ~Interpreter() {
            if (_ts) {
                SwapState swap_state(_ts);
                Py_EndInterpreter(_ts);
            }
        }

        PyInterpreterState *interp() {
            return _ts->interp;
        }

    private:
        PyThreadState *_ts;
    };

    struct EnableThread {
        EnableThread() {
            _state = PyEval_SaveThread();
        }

        ~EnableThread() {
            PyEval_RestoreThread(_state);
        }

    private:
        PyThreadState *_state;
    };

    PythonScriptModulePrivate(const std::string &content) : _content(content) {}

    bool execute(std::shared_ptr<ModuleContext> context) {
        bool ret;
        auto state = PyGILState_Ensure();
        ret = ::execute(_content, context);
        PyGILState_Release(state);
        return ret;
    }

//    bool execute_thread(const std::string &content) {
//        Interpreter interpreter;
//
//        std::thread exec(execute_in_thread, interpreter.interp(), content);
//        EnableThread enable;
//        exec.join();
//        return true;
//    }
private:
//    static void execute_in_thread(PyInterpreterState *interp, const std::string &content) {
//        Interpreter::Thread scope(interp);
//
//        ::execute(content);
//    }
    const std::string _content;
};

PythonScriptModule::PythonScriptModule(const std::string &content, const std::string &name, double version)
        : ScriptModule(name, version), d(new PythonScriptModulePrivate(content)) {
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

PythonFileScriptModule::PythonFileScriptModule(const std::string &file_name, const std::string &name, double version)
        : ScriptModule(name, version) {
    std::ifstream script(file_name, std::ios_base::in);
    if (script.is_open()) {
        auto content = std::string(std::istreambuf_iterator<char>(script), std::istreambuf_iterator<char>());
        d = std::shared_ptr<PythonScriptModule>(new PythonScriptModule(content, name, version));
        script.close();
    } else {
        fprintf(stderr, "Script %s can't read\n", file_name.c_str());
    }
}

PythonFileScriptModule::~PythonFileScriptModule() {

}

bool
PythonFileScriptModule::execute(std::shared_ptr<ModuleContext> context) {
    return d ? d->execute(context) : false;
}
