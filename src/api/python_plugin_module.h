#ifndef ALGORITHM_PYTHON_PLUGIN_MODULE_H
#define ALGORITHM_PYTHON_PLUGIN_MODULE_H
#include <string>
#include <memory>
#include <plugin_api.h>

class PythonScriptModulePrivate;

class PythonScriptModule final: public ScriptModule {
public:
    PythonScriptModule(const std::string &content, const std::string &name = "unregistered", double version = 0.0);
    ~PythonScriptModule();

    bool execute(std::shared_ptr<ModuleContext> context) override;
private:
    std::shared_ptr<PythonScriptModulePrivate> d;
};

class PythonFileScriptModule: public ScriptModule {
public:
    PythonFileScriptModule(const std::string &file_name, const std::string &name = "unregistered", double version = 0.0);
    ~PythonFileScriptModule();

    bool execute(std::shared_ptr<ModuleContext> context) override;
private:
    std::shared_ptr<PythonScriptModule> d;
};

#endif //ALGORITHM_PYTHON_PLUGIN_MODULE_H
