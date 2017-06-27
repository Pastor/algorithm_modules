#include <gtest/gtest.h>
#include <python_plugin_module.h>


TEST(PythonModule, ScriptModule) {
    PythonScriptModule module(std::string("def main(context, name):\n    return 0"));

    const auto &context = std::shared_ptr<ModuleContext>(new ModuleContext);
    ASSERT_TRUE(module.execute(context));
}

TEST(PythonModule, FileModule) {
    PythonFileScriptModule module("tests_data/plugin.01.py");

    const auto &context = std::shared_ptr<ModuleContext>(new ModuleContext);
    context->set_property("key", "value");
    ASSERT_TRUE(module.execute(context));
}
