#include <gtest/gtest.h>
#include <plugin_manager.h>
#include <python_plugin_module.h>
#include "tests.h"

const std::string script_file_path = "tests_data/plugin.01.py";

TEST(PythonModule, ScriptModule) {
    PythonScriptModule module(std::string("def main(context, name):\n    return 0"));

    const auto &context = std::shared_ptr<ModuleContext>(new ModuleContext);
    ASSERT_TRUE(module.execute(context));
}

TEST(PythonModule, FileModule) {
    PythonFileScriptModule module(script_file_path);

    const auto &context = std::shared_ptr<ModuleContext>(new ModuleContext);
    context->set_property("key", "value");
    ASSERT_TRUE(module.execute(context));
}

TEST(PythonModule, /*DISABLED_*/PluginManager) {
    auto context = std::shared_ptr<ModuleContext>(new ModuleContext);
    PluginManager manager;

    manager.load(configuration_file);
    PluginSpec spec;
    spec.plugin_file_path = script_file_path;
    spec.plugin_stage = PluginSpec::FirstInput;
    spec.plugin_version = 1.02;
    spec.plugin_name = "PythonModule";
    spec.plugin_description = "Описание";
    spec.plugin_type = PluginSpec::PythonScript;

    manager.register_module(spec);

    manager.execute(spec.plugin_name, context);
}
