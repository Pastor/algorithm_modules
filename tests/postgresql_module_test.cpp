#include <gtest/gtest.h>
#include <plugin_api.h>
#include <dynamic_library_module.h>

TEST(PostgreSqlModule, Call) {
    DynamicLibraryModule module("postgresql_module.dll");

    auto context = std::shared_ptr<ModuleContext>(new ModuleContext);
    context->set_property(Constants::Database_Hostname, "localhost");
    context->set_property(Constants::Database_Database, "operational_db");
    context->set_property(Constants::Database_Username, "dcm");
    context->set_property(Constants::Database_Password, "12345678");
    context->set_property(Constants::Database_Port, "5432");
    ASSERT_TRUE(module.execute(context));
}