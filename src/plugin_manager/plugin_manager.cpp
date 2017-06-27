#include <string>
#include <map>
#include <plugin_manager.h>
#include <dynamic_library_module.h>
#include <python_plugin_module.h>

struct PluginManagerPrivate {
    std::map<std::string, std::pair<PluginSpec, std::shared_ptr<Module>>> modules;
};

void
PluginManager::register_dynamic(const std::string &file_name) {
    register_module(std::shared_ptr<Module>(new DynamicLibraryModule(file_name)));
}

void
PluginManager::register_script(const std::string &name,
                               const std::string &description,
                               double version,
                               const std::string &file_name) {
    register_module(std::shared_ptr<Module>(new PythonFileScriptModule(file_name, name, description, version)));
}

void
PluginManager::execute(const std::string &name, std::shared_ptr<ModuleContext> context) {
    auto &module = d->modules[name];
    if (module.second) {
        module.second->execute(context);
    }
}

void
PluginManager::register_module(std::shared_ptr<Module> module) {
    auto &spec = module->spec();
    d->modules[spec.plugin_name] = std::make_pair(spec, module);
}

PluginManager::PluginManager()
        : d(new PluginManagerPrivate) {

}

bool
PluginManager::contains(const std::string &name) const {
    const auto &it = d->modules.find(name);
    return it != d->modules.end() && it->second.second;
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
}

void
PluginManager::register_module(const PluginSpec &spec) {

    if (!spec.is_valid()) {
        fprintf(stdout, "Can't register not valid module\n");
        return;
    }
    switch (spec.plugin_type) {
        case PluginSpec::DynamicLibrary:
            register_dynamic(spec.plugin_file_path);
            break;
        case PluginSpec::PythonScript:
            register_script(spec.plugin_name, spec.plugin_description, spec.plugin_version, spec.plugin_file_path);
            break;
        default:
            fprintf(stdout, "Can't register unknown module\n");
            break;
    }
}


