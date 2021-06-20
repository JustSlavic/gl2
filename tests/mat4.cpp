#include <gtest/gtest.h>
#include <math.hpp>


TEST(Matrix4, Zero) {
    auto result = math::matrix4::zero();
    auto expected = math::matrix4();

    for (int i = 0; i < 16; i++) {
        expected.at[i] = 0;
    }

    ASSERT_EQ(result, expected);
}

TEST(Matrix4, Eye) {
    auto result = math::matrix4::identity();
    auto expected = math::matrix4::zero();
    expected._11 = expected._22 = expected._33 = expected._44 = 1;

    ASSERT_EQ(result, expected);
}
