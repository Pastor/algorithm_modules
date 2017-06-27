#include <gtest/gtest.h>
#include <plugin_api.h>
#include <postgresql_context.h>
#include "tests.h"

TEST(PostgreSqlContext, Connect) {
    PostgreSqlContext context;

    ASSERT_TRUE(context.connect());
    ASSERT_TRUE(context.type() == SqlContext::PostgreSQL);
}

TEST(PostgreSqlContext, IsConnected) {
    PostgreSqlContext context;

    EXPECT_FALSE(context.is_connected());
    EXPECT_TRUE(context.connect());
    ASSERT_TRUE(context.is_connected());
}
