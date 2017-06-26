#include <gtest/gtest.h>
#include <plugin_api.h>


TEST(ModuleContext, Properties) {
    ModuleContext ctx;

    ctx.set_property("int_value", "20");
    ctx.set_property("bool_value", "true");
    ctx.set_property("string_value", "string");

    ASSERT_EQ(ctx.int_value("int_value"), 20);
    ASSERT_EQ(ctx.bool_value("bool_value"), true);
    ASSERT_STREQ(ctx.property("string_value").c_str(), "string");
}
