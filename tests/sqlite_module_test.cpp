#include <gtest/gtest.h>
#include <plugin_api.h>
#include <dynamic_library_module.h>
#include <plugin_manager.h>
#include "tests.h"

TEST(SqliteModule, Call) {
    DynamicLibraryModule module("sqlite_module.dll");

    ASSERT_TRUE(module.execute(std::shared_ptr<ModuleContext>(new ModuleContext)));
}

TEST(SqliteModule, PluginManager) {
    auto context = std::shared_ptr<ModuleContext>(new ModuleContext);
    PluginManager manager;
    manager.load(configuration_file);
    PluginSpec spec;
    spec.plugin_file_path = "sqlite_module.dll";
    spec.plugin_stage = PluginSpec::FirstInput;
    spec.plugin_version = 1.02;
    spec.plugin_name = "SqliteModule";
    spec.plugin_description = "Описание";
    spec.plugin_type = PluginSpec::DynamicLibrary;

    manager.register_module(spec);

    manager.execute(spec.plugin_name, context);
}
