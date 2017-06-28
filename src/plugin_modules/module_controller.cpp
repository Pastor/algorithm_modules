#include <memory>
#include <thread>
#include <tinyxml2.h>
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
        //Создаем схему и инфраструктуру
        module->execute(context);
        //Сохраняем результат
    }
}

void
ModuleController::execute(const PluginSpec &spec, std::shared_ptr<ModuleContext> context) {
    if (spec.is_valid() && context) {
        std::shared_ptr<ModuleContext> module_context(_context);
        context->copy_to(*module_context);
        if (spec.plugin_context) {
            spec.plugin_context->copy_to(*module_context);
        }
        std::thread caller(module_call, spec, module_context);
        caller.detach();
    }
}

ModuleController::ModuleController(const std::shared_ptr<ModuleContext> &context)
        : _context(context) {}

void
ModuleController::load(const std::string &file_path) {
    tinyxml2::XMLDocument document;

    document.LoadFile(file_path.c_str());
    const auto configuration_xml = document.FirstChildElement("Configuration");
    if (configuration_xml == nullptr)
        return;
    for (auto node = configuration_xml->FirstChild(); node; node = node->NextSibling()) {
        const auto element = node->ToElement();
        if (element == nullptr)
            continue;
        fprintf(stdout, "ModuleController. Name: %s\n", element->Name());
        if (std::strcmp(element->Name(), "Properties") == 0) {
            _context->fromXml(element);
        }
    }
}
