#include <memory>
#include <thread>
#include <dynamic_library_module.h>
#include <python_plugin_module.h>
#include "module_controller.h"

static std::shared_ptr<Module>
load_from_spec(const PluginSpec &spec) {
    switch (spec.plugin_type) {
        case PluginSpec::DynamicLibrary:
            return std::shared_ptr<Module>(new DynamicLibraryModule(spec.plugin_file_path));
        case PluginSpec::PythonScript:
            return std::shared_ptr<Module>(
                    new PythonFileScriptModule(
                            spec.plugin_file_path, spec.plugin_name, spec.plugin_description, spec.plugin_version));
        default:
            return std::shared_ptr<Module>(nullptr);
    }
}

static void
module_call(const PluginSpec &spec, std::shared_ptr<ModuleContext> context) {
    auto module = load_from_spec(spec);
    if (module) {
        module->execute(context);
    }
}

void
ModuleController::execute(const PluginSpec &spec, std::shared_ptr<ModuleContext> context) {
    if (spec.is_valid() && context) {
        std::thread caller(module_call, spec, context);
        caller.detach();
    }
}
