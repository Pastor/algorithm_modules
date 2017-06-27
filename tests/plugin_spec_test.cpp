#include <gtest/gtest.h>
#include <plugin_spec.h>
#include "tests.h"

TEST(PluginSpec, Validate) {
    PluginSpec spec;
    spec.plugin_version = 1.02;
    spec.plugin_description = "Динамическая библиотека(тестовая)";
    spec.plugin_file_path = library;
    spec.plugin_name = "TestDynamic";
    spec.plugin_type = PluginSpec::DynamicLibrary;

    ASSERT_TRUE(spec.is_valid());
    spec.plugin_type = PluginSpec::UnknownModule;
    ASSERT_FALSE(spec.is_valid());
    spec.plugin_type = PluginSpec::DynamicLibrary;
    spec.plugin_name = "";
    ASSERT_FALSE(spec.is_valid());
}

TEST(PluginSpecController, Add) {
    PluginSpecController controller("configuration.xml");

    PluginSpec spec;
    spec.plugin_version = 1.02;
    spec.plugin_description = "Динамическая библиотека(тестовая)";
    spec.plugin_file_path = library;
    spec.plugin_name = "TestDynamic";
    spec.plugin_type = PluginSpec::DynamicLibrary;
    controller.add(spec);
    ASSERT_TRUE(controller.specs().size() == 1);
}

TEST(PluginSpecController, Read) {
    PluginSpecController controller(configuration_file);

    controller.read();
    ASSERT_TRUE(controller.specs().size() == 1);
    const PluginSpec &spec = controller.specs().at(0);
    ASSERT_DOUBLE_EQ(spec.plugin_version, 1.02);
    ASSERT_STREQ(spec.plugin_name.c_str(), "TestDynamic");
    ASSERT_STREQ(spec.plugin_file_path.c_str(), library.c_str());
}

TEST(PluginSpecController, Write) {
    PluginSpecController read_controller(configuration_file);
    PluginSpecController write_controller("tests_data/configuration1.xml");

    read_controller.read();
    ASSERT_TRUE(read_controller.specs().size() == 1);
    auto &specs = read_controller.specs();
    for (auto it = specs.begin(); it != specs.end(); ++it) {
        write_controller.add((*it));
    }
    write_controller.write();
    ASSERT_TRUE(write_controller.specs().size() == 1);
    write_controller.read();
    ASSERT_TRUE(write_controller.specs().size() == 1);
}
