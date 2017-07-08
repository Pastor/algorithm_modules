#include <gtest/gtest.h>
#include <plugin_manager.h>
#include "tests.h"

TEST(LoadingModules, Repeat) {
    PluginManager manager;
    auto context = std::shared_ptr<ModuleContext>(new ModuleContext);

    manager.load(configuration_file);
    for (int i = 0; i < 10; ++i) {
        manager.execute("DynamicLibraryModule", context);
        manager.execute("SystemProcessModule", context);
//        manager.execute("PythonModule", context);
    }
}

