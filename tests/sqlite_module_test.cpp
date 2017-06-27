#include <gtest/gtest.h>
#include <plugin_api.h>
#include <dynamic_library_module.h>

TEST(SqliteModule, Call) {
    DynamicLibraryModule module("sqlite_module.dll");

    ASSERT_TRUE(module.execute(std::shared_ptr<ModuleContext>(new ModuleContext)));
}

