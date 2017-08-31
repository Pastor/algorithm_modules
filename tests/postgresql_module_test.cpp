#include <gtest/gtest.h>
#include <plugin_api.h>
#include <plugin_manager.h>
#include <dynamic_library_module.h>
#include "tests.h"

TEST(PostgreSqlModule, Call) {
    DynamicLibraryModule module("postgresql_module.dll");

    auto context = std::make_shared<ModuleContext>();
    context->set_property(Constants::Database_Hostname, TEST_DATABASE_HOSTNAME);
    context->set_property(Constants::Database_Database, TEST_DATABASE_DATABASE);
    context->set_property(Constants::Database_Username, TEST_DATABASE_USERNAME);
    context->set_property(Constants::Database_Password, TEST_DATABASE_PASSWORD);
    context->set_property(Constants::Database_Port, TEST_DATABASE_PORT);
    ASSERT_TRUE(module.execute(context));
}

TEST(PostgreSqlModule, PluginManager) {
    auto context = std::make_shared<ModuleContext>();
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
