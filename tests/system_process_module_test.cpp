#include <gtest/gtest.h>
#include <plugin_api.h>
#include <system_process_module.h>
#include <plugin_manager.h>
#include "tests.h"

#ifndef PythonInterpretator
#define PythonInterpretator "python.exe"
#endif

#define _STRINGIFY(text)  #text
#define STRINGIFY(text)  _STRINGIFY(text)

TEST(SystemProcessModule, Execute) {
    SystemProcessModule module(STRINGIFY(PythonInterpretator) " --version", "name", "description", 1.02);

    const auto &context = std::make_shared<ModuleContext>();
    context->set_property("Version", "1.02");
    context->set_property("Name", "SystemProcessModule");
    ASSERT_TRUE(module.execute(context));
}

TEST(SystemProcessModule, ExecuteWithPutEnvironment) {
    SystemProcessModule module(STRINGIFY(PythonInterpretator) " --version", "name", "description", 1.02);

    const auto &context = std::make_shared<ModuleContext>();
    context->set_property("Version", "1.02");
    context->set_property("Name", "SystemProcessModule");
    context->set_property(Constants::SystemProcessModule_PutEnvironment, "true");
    ASSERT_TRUE(module.execute(context));
}

TEST(SystemProcessModule, PluginManager) {
    PluginManager manager;

    manager.load(configuration_file);
    auto context = std::make_shared<ModuleContext>();
    PluginSpec spec;
    spec.plugin_file_path = STRINGIFY(PythonInterpretator) " --version";
    spec.plugin_stage = PluginSpec::FirstInput;
    spec.plugin_version = 1.02;
    spec.plugin_name = "SystemProcessModule";
    spec.plugin_description = "Описание";
    spec.plugin_type = PluginSpec::SystemProcess;

    manager.register_module(spec);
    manager.execute(spec.plugin_name, context);
}

TEST(SystemProcessModule, PluginManagerEcho) {
    PluginManager manager;

    manager.load(configuration_file);
    auto context = std::make_shared<ModuleContext>();
    PluginSpec spec;
    spec.plugin_file_path = TEST_ECHO_SCRIPT;
    spec.plugin_stage = PluginSpec::FirstInput;
    spec.plugin_version = 1.02;
    spec.plugin_name = "SystemProcessModule";
    spec.plugin_description = "Проверка передачи environment";
    spec.plugin_type = PluginSpec::SystemProcess;

    context->set_property(Constants::SystemProcessModule_PutEnvironment, "true");
    manager.register_module(spec);
    manager.execute(spec.plugin_name, context);
}
