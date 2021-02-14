#include <gtest/gtest.h>
#include <defines.h>
#include <service/str_id.hpp>


TEST(String_Id, Equal_Not_Equal) {
    service::str_id id1("SHADER_TRIANGLE");
    service::str_id id2("SHADER_TRIANGLE");
    service::str_id id3("SHADER_ARROW");

    EXPECT_EQ(id1, id2);
    EXPECT_NE(id1, id3);
}

// TEST(String_Id, Overload) {
//     for (size_t i = 0; i < 78; i++) { // 79 is too much
//         service::str_id id("UI_SHADER_" + std::to_string(i));
//     }
// }

