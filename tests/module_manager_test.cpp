#include <gtest/gtest.h>
#include <plugin_manager.h>
#include "tests.h"

TEST(PluginManager, RegisterSpec) {
    PluginManager manager;
    PluginSpec spec;

    spec.plugin_name = "DynamicLibraryModule";
    spec.plugin_file_path = "dynamic_module_library.dll";
    spec.plugin_description = "Динамическая библиотека(тестовая)";
    spec.plugin_version = 1.02;
    spec.plugin_type = PluginSpec::DynamicLibrary;
    manager.register_module(spec);
    ASSERT_TRUE(manager.contains("DynamicLibraryModule"));
}

TEST(PluginManager, Load) {
    PluginManager manager;

    manager.load(configuration_file);
    ASSERT_TRUE(manager.contains("DynamicLibraryModule"));
}

TEST(PluginManager, Execute) {
    PluginManager manager;

    manager.load(configuration_file);
    ASSERT_TRUE(manager.contains("DynamicLibraryModule"));
    auto context = std::shared_ptr<ModuleContext>(new ModuleContext);

    context->set_property(Constants::Stream_Input, "TestInputStream");
    manager.execute("DynamicLibraryModule", context);
}
