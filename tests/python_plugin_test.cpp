#include <gtest/gtest.h>
#include <plugin_manager.h>
#include <python_plugin_module.h>
#include "tests.h"

static const std::string script_file_path1 = "tests_data/plugin.01.py";
static const std::string script_file_path2 = "tests_data/plugin.02.py";

TEST(PythonModule, ScriptModule) {
    PythonScriptModule module(std::string("def main(context, name):\n    return 0"));

    const auto &context = std::make_shared<ModuleContext>();
    ASSERT_TRUE(module.execute(context));
}

TEST(PythonModule, FileModuleScript) {
    PythonFileScriptModule module(script_file_path1);

    const auto &context = std::make_shared<ModuleContext>();
    context->set_property("key", "value");
    ASSERT_TRUE(module.execute(context));
}

TEST(PythonModule, /*DISABLED_*/PluginManager) {
    auto context = std::make_shared<ModuleContext>();
    PluginManager manager;

    manager.load(configuration_file);
    PluginSpec spec;
    spec.plugin_file_path = script_file_path2;
    spec.plugin_stage = PluginSpec::FirstInput;
    spec.plugin_version = 1.02;
    spec.plugin_name = "PythonModule";
    spec.plugin_description = "Описание";
    spec.plugin_type = PluginSpec::PythonScript;

    manager.register_module(spec);

    manager.execute(spec.plugin_name, context);
}
