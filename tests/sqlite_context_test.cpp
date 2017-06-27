#include <gtest/gtest.h>
#include <plugin_api.h>
#include <sqlite_context.h>

TEST(SqliteContext, Connect) {
    SqliteContext context;

    ASSERT_TRUE(context.connect());
    ASSERT_TRUE(context.type() == SqlContext::SQLite);
}

TEST(SqliteContext, IsConnected) {
    SqliteContext context;

    EXPECT_FALSE(context.is_connected());
    EXPECT_TRUE(context.connect());
    ASSERT_TRUE(context.is_connected());
}
