#include <gtest/gtest.h>
#include <plugin_api.h>
#include <plugin_manager.h>
#include <dynamic_library_module.h>

TEST(PostgreSqlModule, Call) {
    DynamicLibraryModule module("postgresql_module.dll");

    auto context = std::shared_ptr<ModuleContext>(new ModuleContext);
    context->set_property(Constants::Database_Hostname, "localhost");
    context->set_property(Constants::Database_Database, "operational_db");
    context->set_property(Constants::Database_Username, "postgres");
    context->set_property(Constants::Database_Password, "123456");
    context->set_property(Constants::Database_Port, "5432");
    ASSERT_TRUE(module.execute(context));
}

TEST(PostgreSqlModule, PluginManager) {
    DynamicLibraryModule module("postgresql_module.dll");
    PluginManager manager;

    manager.register_module(module.spec());
    auto context = std::shared_ptr<ModuleContext>(new ModuleContext);
    context->set_property(Constants::Database_Hostname, "localhost");
    context->set_property(Constants::Database_Database, "operational_db");
    context->set_property(Constants::Database_Username, "postgres");
    context->set_property(Constants::Database_Password, "123456");
    context->set_property(Constants::Database_Port, "5432");
    manager.execute(module.spec().plugin_name, context);
}
