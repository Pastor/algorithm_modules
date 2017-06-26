#include <gtest/gtest.h>
#include <plugin_manager.h>
#include <dynamic_library_module.h>


TEST(PluginManager, Register) {
    PluginManager manager;
    auto module = std::shared_ptr<Module>(new DynamicLibraryModule("libdynamic_module_library.dll"));

    manager.register_module(module);
    ASSERT_TRUE(manager.contains("dynamic_module_library"));
}
