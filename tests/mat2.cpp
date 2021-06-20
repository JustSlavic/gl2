#include <gtest/gtest.h>
#include <math.hpp>


TEST(Matrix2, Zero) {
    auto result = math::matrix2::zero();
    auto expected = math::matrix2();

    for (int i = 0; i < 4; i++) {
        expected.at[i] = 0;
    }

    ASSERT_EQ(result, expected);
}

TEST(Matrix2, Eye) {
    auto result = math::matrix2::identity();
    auto expected = math::matrix2::zero();
    expected._11 = expected._22 = 1;

    ASSERT_EQ(result, expected);
}

void print(math::matrix2 m) {
    printf(
        "|%f %f|\n"
        "|%f %f|\n",
        m._11, m._12,
        m._21, m._22);
} 

TEST(Matrix2, Multiplication) {
    auto m = math::matrix2{
        2, 4,
        -1, 3
    };

    auto t = math::matrix2{
        -1, 5,
        3, 7
    };

    auto v = math::vector2{ 1, 3 };

    auto v_m = math::vector2{ -1, 13 };
    auto m_v = math::vector2{ 14, 8 };
    auto m_t = math::matrix2{
        2*(-1) + 4*3, 2*5+4*7,
        (-1)*(-1) + 3*3, (-1)*5 + 3*7
    };
    auto t_m = math::matrix2{
        -2-5, -4+15,
        6-7, 12+21
    };
    
    EXPECT_EQ(v * m, v_m);
    EXPECT_EQ(m * v, m_v);
    EXPECT_EQ(m * t, m_t);
    EXPECT_EQ(t * m, t_m);
}
