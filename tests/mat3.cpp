#include <gtest/gtest.h>
#include <math.hpp>


TEST(Matrix3, Zero) {
    auto result = math::matrix3::zero();
    auto expected = math::matrix3();

    for (int i = 0; i < 9; i++) {
        expected.at[i] = 0;
    }

    ASSERT_EQ(result, expected);
}

TEST(Matrix3, Eye) {
    auto result = math::matrix3::identity();
    auto expected = math::matrix3::zero();
    expected._11 = expected._22 = expected._33 = 1;

    ASSERT_EQ(result, expected);
}

TEST(Matrix3, Multiplication) {
    auto m = math::matrix3{
        2, 4, 3,
        -1, 3, 0,
        8, 5, -4
    };

    auto t = math::matrix3{
        -1, 5, 4,
        3, 7, 2,
        0, -5, 1
    };

    auto v = math::vector3{ 1, 3, 13 };

    auto v_m = math::vector3{ -1, 13, -5 };
    auto m_v = math::vector3{ 14, 8 };
    auto m_t = math::matrix3{
        2*(-1) + 4*3, 2*5+4*7,
        (-1)*(-1) + 3*3, (-1)*5 + 3*7
    };
    auto t_m = math::matrix3{
        -2-5, -4+15,
        6-7, 12+21
    };
    
    EXPECT_EQ(v * m, v_m);
    EXPECT_EQ(m * v, m_v);
    EXPECT_EQ(m * t, m_t);
    EXPECT_EQ(t * m, t_m);
}
