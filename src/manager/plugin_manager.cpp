#include <string>
#include <map>
#include <plugin_manager.h>
#include <dynamic_library_module.h>
#include <python_plugin_module.h>

struct PluginManagerPrivate {
    std::map<std::string, std::shared_ptr<Module>> modules;
};

void
PluginManager::register_dynamic(const std::string &file_name) {
    register_module(std::shared_ptr<Module>(new DynamicLibraryModule(file_name)));
}

void
PluginManager::register_script(const std::string &name, double version, const std::string &file_name) {
    register_module(std::shared_ptr<Module>(new PythonFileScriptModule(file_name, name, version)));
}

void
PluginManager::execute(const std::string &name, std::shared_ptr<ModuleContext> context) {
    auto &module = d->modules[name];
    if (module) {
        module->execute(context);
    }
}
void
PluginManager::register_module(std::shared_ptr<Module> module) {
    d->modules[module->name()] = module;
}

PluginManager::PluginManager()
        : d(new PluginManagerPrivate) {

}

bool
PluginManager::contains(const std::string &name) const {
    return d->modules.find(name) != d->modules.end();
}


