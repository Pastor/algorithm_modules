#include <gtest/gtest.h>
#include <python_plugin_module.h>


TEST(PythonModule, ScriptModule) {
    PythonScriptModule module(std::string("def main(context, connection):\n    return 0"));

    ASSERT_TRUE(module.execute(std::shared_ptr<ModuleContext>(nullptr)));
}

TEST(PythonModule, FileModule) {
    PythonFileScriptModule module("scripts/plugin.01.py");

    ASSERT_TRUE(module.execute(std::shared_ptr<ModuleContext>(nullptr)));
}
