#include <gtest/gtest.h>
#include <service/str_id.hpp>


TEST(StringID, Equal) {
    service::str_id id1("SHADER_TRIANGLE");
    service::str_id id2("SHADER_TRIANGLE");

    EXPECT_EQ(id1, id2);
}

TEST(StringID, NotEqual) {
    service::str_id id1("SHADER_TRIANGLE");
    service::str_id id2("SHADER_ARROW");

    EXPECT_NE(id1, id2);
}

