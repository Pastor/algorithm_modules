#include <gtest/gtest.h>
#include <plugin_api.h>
#include <plugin_manager.h>
#include <dynamic_library_module.h>
#include "tests.h"

TEST(PostgreSqlModule, Call) {
    DynamicLibraryModule module("postgresql_module.dll");

    auto context = std::shared_ptr<ModuleContext>(new ModuleContext);
    context->set_property(Constants::Database_Hostname, "localhost");
    context->set_property(Constants::Database_Database, "operational_db");
    context->set_property(Constants::Database_Username, "testing");
    context->set_property(Constants::Database_Password, "testing");
    context->set_property(Constants::Database_Port, "5432");
    ASSERT_TRUE(module.execute(context));
}

TEST(PostgreSqlModule, PluginManager) {
    auto context = std::shared_ptr<ModuleContext>(new ModuleContext);
    PluginManager manager;
    manager.load(configuration_file);

    PluginSpec spec;
    spec.plugin_file_path = "postgresql_module.dll";
    spec.plugin_stage = PluginSpec::FirstInput;
    spec.plugin_version = 1.02;
    spec.plugin_name = "PostgreSqlModule";
    spec.plugin_description = "Описание";
    spec.plugin_type = PluginSpec::DynamicLibrary;

    manager.register_module(spec);

    manager.execute(spec.plugin_name, context);
}
