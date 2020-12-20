#include <gtest/gtest.h>
#include <service/string_id/string_id.hpp>

using namespace service;


TEST(StringID, Make) {
    StringID id("SHADER_TRIANGLE");
    EXPECT_TRUE(true);
}

TEST(StringID, Equal) {
    StringID id1("SHADER_TRIANGLE");
    StringID id2("SHADER_TRIANGLE");

    EXPECT_EQ(id1, id2);
}

TEST(StringID, NotEqual) {
    StringID id1("SHADER_TRIANGLE");
    StringID id2("SHADER_ARROW");

    EXPECT_NE(id1, id2);
}

