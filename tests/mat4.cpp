#include <gtest/gtest.h>
#include <math/mat4.hpp>


TEST(Mat4, Zero) {
    auto m = math::mat4::zero();

    EXPECT_EQ(m._11, 0.f); EXPECT_EQ(m._12, 0.f); EXPECT_EQ(m._13, 0.f); EXPECT_EQ(m._14, 0.f);
    EXPECT_EQ(m._21, 0.f); EXPECT_EQ(m._22, 0.f); EXPECT_EQ(m._23, 0.f); EXPECT_EQ(m._24, 0.f);
    EXPECT_EQ(m._31, 0.f); EXPECT_EQ(m._32, 0.f); EXPECT_EQ(m._33, 0.f); EXPECT_EQ(m._34, 0.f);
    EXPECT_EQ(m._41, 0.f); EXPECT_EQ(m._42, 0.f); EXPECT_EQ(m._43, 0.f); EXPECT_EQ(m._44, 0.f);
}

TEST(Mat4, Eye) {
    auto m = math::mat4::eye();

    EXPECT_EQ(m._11, 1.f); EXPECT_EQ(m._12, 0.f); EXPECT_EQ(m._13, 0.f); EXPECT_EQ(m._14, 0.f);
    EXPECT_EQ(m._21, 0.f); EXPECT_EQ(m._22, 1.f); EXPECT_EQ(m._23, 0.f); EXPECT_EQ(m._24, 0.f);
    EXPECT_EQ(m._31, 0.f); EXPECT_EQ(m._32, 0.f); EXPECT_EQ(m._33, 1.f); EXPECT_EQ(m._34, 0.f);
    EXPECT_EQ(m._41, 0.f); EXPECT_EQ(m._42, 0.f); EXPECT_EQ(m._43, 0.f); EXPECT_EQ(m._44, 1.f);
}
