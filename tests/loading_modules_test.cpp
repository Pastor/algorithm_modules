#include <gtest/gtest.h>
#include <plugin_manager.h>
#include "tests.h"

TEST(LoadingModules, Repeat) {
    PluginManager manager;
    auto context = std::make_shared<ModuleContext>();

    manager.load(configuration_file);
    for (int i = 0; i < 10; ++i) {
        manager.execute("DynamicLibraryModule", context);
        manager.execute("SystemProcessModule", context);
//        manager.execute("PythonModule", context);
    }
}

