#include <gtest/gtest.h>
#include <plugin_api.h>
#include <system_process_module.h>
#include "tests.h"

#ifndef PythonInterpretator
#define PythonInterpretator "python.exe"
#endif

#define _STRINGIFY(text)  #text
#define STRINGIFY(text)  _STRINGIFY(text)

TEST(SystemProcessModule, Execute) {
    SystemProcessModule module(STRINGIFY(PythonInterpretator) " --version", "name", "description", 1.02);

    const auto &context = std::shared_ptr<ModuleContext>(new ModuleContext);
    context->set_property("Version", "1.02");
    context->set_property("Name", "SystemProcessModule");
    ASSERT_TRUE(module.execute(context));
}

TEST(SystemProcessModule, ExecuteWithPutEnvironment) {
    SystemProcessModule module(STRINGIFY(PythonInterpretator) " --version", "name", "description", 1.02);

    const auto &context = std::shared_ptr<ModuleContext>(new ModuleContext);
    context->set_property("Version", "1.02");
    context->set_property("Name", "SystemProcessModule");
    context->set_property(Constants::SystemProcessModule_PutEnvironment, "true");
    ASSERT_TRUE(module.execute(context));
}
