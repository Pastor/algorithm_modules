#include <gtest/gtest.h>
#include <plugin_api.h>
#include <plugin_spec.h>
#include "tests.h"

#define MODULES_COUNT 3

TEST(PluginSpec, Validate) {
    PluginSpec spec;
    spec.plugin_version = 1.02;
    spec.plugin_description = "Динамическая библиотека(тестовая)";
    spec.plugin_file_path = library;
    spec.plugin_name = "DynamicLibraryModule";
    spec.plugin_type = PluginSpec::DynamicLibrary;

    ASSERT_TRUE(spec.is_valid());
    spec.plugin_type = PluginSpec::UnknownModule;
    ASSERT_FALSE(spec.is_valid());
    spec.plugin_type = PluginSpec::DynamicLibrary;
    spec.plugin_name = "";
    ASSERT_FALSE(spec.is_valid());
    spec.plugin_name = "DynamicLibraryModule";
    spec.plugin_stage = PluginSpec::ProcessingInput;
    ASSERT_FALSE(spec.is_valid());
    spec.plugin_input_stream = "My";
    ASSERT_TRUE(spec.is_valid());
    spec.plugin_input_stream = "";
    spec.plugin_stage = PluginSpec::FirstInput;
    ASSERT_TRUE(spec.is_valid());
}

TEST(PluginSpecController, Add) {
    PluginSpecController controller("configuration.xml");

    PluginSpec spec;
    spec.plugin_version = 1.02;
    spec.plugin_description = "Динамическая библиотека(тестовая)";
    spec.plugin_file_path = library;
    spec.plugin_name = "DynamicLibraryModule";
    spec.plugin_type = PluginSpec::DynamicLibrary;
    controller.add(spec);
    ASSERT_EQ(controller.specs().size(), 1);
}

TEST(PluginSpecController, Read) {
    PluginSpecController controller(configuration_file);

    controller.read();
    ASSERT_EQ(controller.specs().size(), MODULES_COUNT);
    const PluginSpec &spec = controller.specs().at(0);
    ASSERT_DOUBLE_EQ(spec.plugin_version, 1.02);
    ASSERT_STREQ(spec.plugin_name.c_str(), "DynamicLibraryModule");
    ASSERT_STREQ(spec.plugin_file_path.c_str(), library.c_str());
}

TEST(PluginSpecController, Write) {
    PluginSpecController read_controller(configuration_file);
    PluginSpecController write_controller("tests_data/configuration1.xml");

    read_controller.read();
    ASSERT_EQ(read_controller.specs().size(), MODULES_COUNT);
    auto &specs = read_controller.specs();
    for (auto it = specs.begin(); it != specs.end(); ++it) {
        write_controller.add((*it));
    }
    write_controller.write();
    ASSERT_EQ(write_controller.specs().size(), MODULES_COUNT);
    write_controller.read();
    ASSERT_EQ(write_controller.specs().size(), MODULES_COUNT);
}

TEST(PluginSpecController, Context) {
    PluginSpecController controller(configuration_file);

    controller.read();
    ASSERT_EQ(controller.specs().size(), MODULES_COUNT);
    const PluginSpec &spec = controller.specs().at(0);
    ASSERT_STREQ(spec.plugin_name.c_str(), "DynamicLibraryModule");
    ASSERT_STREQ(spec.plugin_context->property("CreatedAt").c_str(), "2017-06-28T13:45:00.000+0000");
}
