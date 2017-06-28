#include <gtest/gtest.h>
#include <plugin_api.h>
#include <dynamic_library_module.h>
#include <plugin_manager.h>

TEST(SqliteModule, Call) {
    DynamicLibraryModule module("sqlite_module.dll");

    ASSERT_TRUE(module.execute(std::shared_ptr<ModuleContext>(new ModuleContext)));
}

TEST(SqliteModule, PluginManager) {
    PluginManager manager;

    PluginSpec spec;
    spec.plugin_file_path = "sqlite_module.dll";
    spec.plugin_stage = PluginSpec::FirstInput;
    spec.plugin_version = 1.02;
    spec.plugin_name = "SqliteModule";
    spec.plugin_description = "Описание";
    spec.plugin_type = PluginSpec::DynamicLibrary;

    manager.register_module(spec);
    auto context = std::shared_ptr<ModuleContext>(new ModuleContext);
    context->set_property(Constants::Database_Hostname, "localhost");
    context->set_property(Constants::Database_Database, "operational_db");
    context->set_property(Constants::Database_Username, "testing");
    context->set_property(Constants::Database_Password, "testing");
    context->set_property(Constants::Database_Port, "5432");
    context->set_property(Constants::Stream_Input, "TestInputStream");
    manager.execute(spec.plugin_name, context);
}
