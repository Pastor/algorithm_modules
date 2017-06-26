#include <gtest/gtest.h>
#include <plugin_api.h>
#include <dynamic_library_module.h>

const std::string library = "libdynamic_module_library.dll";

TEST(DynamicLibraryModule, Loaded) {
    DynamicLibraryModule module(library);

    ASSERT_TRUE(module.is_loaded());
}


TEST(DynamicLibraryModule, Name) {
    DynamicLibraryModule module(library);

    ASSERT_STREQ(module.name().c_str(), "dynamic_module_library");
}

TEST(DynamicLibraryModule, Version) {
    DynamicLibraryModule module(library);

    ASSERT_EQ(module.version(), 1.02);
}


TEST(DynamicLibraryModule, Call) {
    DynamicLibraryModule module(library);

    ASSERT_TRUE(module.execute(std::shared_ptr<ModuleContext>(new ModuleContext)));
}