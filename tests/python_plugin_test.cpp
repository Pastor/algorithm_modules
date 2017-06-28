#include <gtest/gtest.h>
#include <plugin_manager.h>
#include <python_plugin_module.h>

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

TEST(PythonModule, PluginManager) {
    PluginManager manager;

    PluginSpec spec;
    spec.plugin_file_path = script_file_path;
    spec.plugin_stage = PluginSpec::FirstInput;
    spec.plugin_version = 1.02;
    spec.plugin_name = "PythonModule";
    spec.plugin_description = "Описание";
    spec.plugin_type = PluginSpec::PythonScript;

    manager.register_module(spec);
    auto context = std::shared_ptr<ModuleContext>(new ModuleContext);
    context->set_property(Constants::Database_Hostname, "localhost");
    context->set_property(Constants::Database_Database, "operational_db");
    context->set_property(Constants::Database_Username, "postgres");
    context->set_property(Constants::Database_Password, "123456");
    context->set_property(Constants::Database_Port, "5432");
    context->set_property(Constants::Stream_Input, "TestInputStream");
    manager.execute(spec.plugin_name, context);
}
