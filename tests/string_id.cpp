#include <gtest/gtest.h>
#include <defines.hpp>
#include <service/string_id.hpp>


TEST(String_Id, Equal_Not_Equal) {
    service::string_id id1("SHADER_TRIANGLE");
    service::string_id id2("SHADER_TRIANGLE");
    
    { service::string_id id_triangle("SHADER_TRIANGLE"); }
    { service::string_id id_triangle("SHADER_TRIANGLE"); }
    { service::string_id id_triangle("SHADER_TRIANGLE"); }
    { service::string_id id_triangle("SHADER_TRIANGLE"); }
    { service::string_id id_triangle("SHADER_TRIANGLE"); }

    service::string_id id3("SHADER_ARROW");

    EXPECT_EQ(id1, id2);
    EXPECT_NE(id1, id3);
}

// TEST(String_Id, Overload) {
//     for (size_t i = 0; i < 78; i++) { // 79 is too much
//         service::string_id id("UI_SHADER_" + std::to_string(i));
//     }
// }

