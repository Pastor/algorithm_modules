#include <memory>
#include <thread>
#include <tinyxml2.h>
#include <dynamic_library_module.h>
#include <algorithm>
#include <python_plugin_module.h>
#include <system_process_module.h>
#include "plugin_controller.h"
#include "plugin_controller_service.h"

static std::shared_ptr<Module>
load_from_spec(const PluginSpec &spec) {
    switch (spec.plugin_type) {
        case PluginSpec::DynamicLibrary:
            return std::shared_ptr<Module>(new DynamicLibraryModule(spec.plugin_file_path));
        case PluginSpec::PythonScript:
            return std::shared_ptr<Module>(
                    new PythonFileScriptModule(
                            spec.plugin_file_path, spec.plugin_name, spec.plugin_description, spec.plugin_version));
        case PluginSpec::SystemProcess:
            return std::shared_ptr<Module>(
                    new SystemProcessModule(
                            spec.plugin_file_path, spec.plugin_name, spec.plugin_description, spec.plugin_version)
            );
        default:
            return std::shared_ptr<Module>(nullptr);
    }
}

static void
module_call(const PluginSpec &spec, std::shared_ptr<ModuleContext> &context, std::shared_ptr<BaseConnection> c) {
    auto module = load_from_spec(spec);
    if (module) {
        ControllerService service(spec, c);

        fprintf(stdout, "[ModuleController] Module %s loaded successful\n", spec.plugin_name.c_str());
        if (service) {
            const auto user_name_text = service.user_name;
            if (!service.is_user_exists()) {
                fprintf(stdout, "[ModuleController] User %s not exists\n", user_name_text.c_str());
                if (service.create_user()) {
                    fprintf(stdout, "[ModuleController] User %s created\n", user_name_text.c_str());
                } else {
                    fprintf(stderr, "[ModuleController] User %s can't create\n", user_name_text.c_str());
                }
            } else {
                if (!service.update_password()) {
                    fprintf(stderr, "[ModuleController] Can't retrieved password \n");
                }
            }
            if (service.user_exists) {
                context->set_property(Constants::Database_Username, service.user_name);
                context->set_property(Constants::Database_Password, service.user_password);
                context->set_property(Constants::Database_Schema, service.schema_name);

                auto stream_input = context->property(Constants::Database_Stream_Input);
                if (spec.plugin_stage == PluginSpec::FirstInput)
                    stream_input = context->property(Constants::Stream_Input);
                const auto stage = spec.plugin_stage_text();
                if (service.create_output_table(stream_input, stage)) {
                    context->set_property(Constants::Database_Stream_Output, service.output_table);
                    module->execute(context);
                    service.update_complete();
                }
            }
        } else {
            fprintf(stderr, "[ModuleController] Can't connecting to system database\n");
        }
    } else {
        fprintf(stderr, "[ModuleController] Module %s not loaded\n", spec.plugin_name.c_str());
    }
}

struct ModuleControllerPrivate final {
    explicit ModuleControllerPrivate(const std::shared_ptr<ModuleContext> &context)
            : c(new BaseConnection(context)) {

    }

    std::shared_ptr<BaseConnection> c;
};

void
ModuleController::execute(const PluginSpec &spec, std::shared_ptr<ModuleContext> context) {
    if (spec.is_valid() && context) {
        std::shared_ptr<ModuleContext> module_context(d->c->copy_context());
        context->copy_to(*module_context);
        if (spec.plugin_context) {
            spec.plugin_context->copy_to(*module_context);
        }
        if (!d->c->is_connected()) {
            auto ret = d->c->reconnect();
            fprintf(stdout, "[ModuleController] BaseConnection not connected. Try connection: %s\n",
                    ret ? "true" : "false");
        }
        std::thread caller(module_call, spec, module_context, d->c);
        caller.join();
    } else {
        fprintf(stderr, "[ModuleController] Module %s not valid or invalid context.\n", spec.plugin_name.c_str());
    }
}

ModuleController::ModuleController(const std::shared_ptr<ModuleContext> &context)
        : d(new ModuleControllerPrivate(context)) {}

void
ModuleController::load(const std::string &file_path) {
    tinyxml2::XMLDocument document;

    document.LoadFile(file_path.c_str());
    const auto configuration_xml = document.FirstChildElement("Configuration");
    if (configuration_xml == nullptr)
        return;
    for (auto node = configuration_xml->FirstChild(); node != nullptr; node = node->NextSibling()) {
        const auto element = node->ToElement();
        if (element == nullptr)
            continue;
        fprintf(stdout, "ModuleController. Name: %s\n", element->Name());
        if (std::strcmp(element->Name(), "Properties") == 0) {
            d->c->load_context(element);
        }
    }
}
