#include <string>
#include <map>
#include <plugin_manager.h>
#include <dynamic_library_module.h>
#include <plugin_controller.h>

struct PluginManagerPrivate final {
    std::map<std::string, PluginSpec> modules;
    std::unique_ptr<ModuleController> module_controller;

    PluginManagerPrivate(const std::shared_ptr<ModuleContext> &context)
            : module_controller(new ModuleController(context)) {}
};

void
PluginManager::register_dynamic(const std::string &file_name) {
    DynamicLibraryModule module(file_name);
    register_module(module.spec());
}

void
PluginManager::register_script(const std::string &name,
                               const std::string &description,
                               double version,
                               const std::string &file_name) {
    PluginSpec spec;
    spec.plugin_name = name;
    spec.plugin_type = PluginSpec::PythonScript;
    spec.plugin_description = description;
    spec.plugin_version = version;
    spec.plugin_file_path = file_name;
    register_module(spec);
}

void
PluginManager::execute(const std::string &name, const std::shared_ptr<ModuleContext> &context) {
    auto &module = d->modules[name];
    if (module.is_valid()) {
        (*d->module_controller).execute(module, context);
    } else {
        fprintf(stderr, "[PluginManager] Module %s not valid.\n", module.plugin_name.c_str());
    }
}

PluginManager::PluginManager(std::shared_ptr<ModuleContext> context)
        : d(new PluginManagerPrivate(context)) {

}

PluginManager::PluginManager()
        : d(new PluginManagerPrivate(std::make_shared<ModuleContext>())){

}


bool
PluginManager::contains(const std::string &name) const {
    const auto &it = d->modules.find(name);
    return it != d->modules.end() && it->second.is_valid();
}

void
PluginManager::load(const std::string &file_name) {
    PluginSpecController controller(file_name);

    controller.read();
    d->modules.clear();
    auto &specs = controller.specs();
    for (auto it = specs.begin(); it != specs.end(); ++it) {
        register_module((*it));
    }
    (*d->module_controller).load(file_name);
}

void
PluginManager::register_module(const PluginSpec &spec) {
    if (!spec.is_valid()) {
        fprintf(stdout, "[PluginManager] Can't register not valid module \n");
        spec.dump("PluginManager");
        return;
    }
    d->modules[spec.plugin_name] = spec;
}

const PluginSpec &
PluginManager::spec(const std::string &name) const {
    static const PluginSpec empty = PluginSpec();
    const auto it = d->modules.find(name);
    if (it != d->modules.end() && it->second.is_valid())
        return (*it).second;
    return empty;
}

