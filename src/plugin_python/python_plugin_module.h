#ifndef ALGORITHM_PYTHON_PLUGIN_MODULE_H
#define ALGORITHM_PYTHON_PLUGIN_MODULE_H
#include <string>
#include <memory>
#include <plugin_api.h>

#if defined(EXPORT_PYTHON_LIBRARY)
#define PYTHON_LIBRARY_API __declspec(dllexport)
#else
#define PYTHON_LIBRARY_API __declspec(dllimport)
#endif

struct PythonScriptModulePrivate;

class PYTHON_LIBRARY_API PythonScriptModule final: public ScriptModule {
public:
    PythonScriptModule(const std::string &content,
                       const std::string &name = "unregistered",
                       const std::string &description = "unregistered",
                       double version = 0.0);
    ~PythonScriptModule();

    bool execute(std::shared_ptr<ModuleContext> context) override;
private:
    std::shared_ptr<PythonScriptModulePrivate> d;
};

class PYTHON_LIBRARY_API PythonFileScriptModule: public ScriptModule {
public:
    PythonFileScriptModule(const std::string &file_name,
                           const std::string &name = "unregistered",
                           const std::string &description = "unregistered",
                           double version = 0.0);
    ~PythonFileScriptModule();

    bool execute(std::shared_ptr<ModuleContext> context) override;
private:
    std::shared_ptr<PythonScriptModule> d;
};

#endif //ALGORITHM_PYTHON_PLUGIN_MODULE_H
