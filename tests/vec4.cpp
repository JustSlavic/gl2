#include <gtest/gtest.h>

#include <math/vector2.hpp>
#include <math/vector3.hpp>
#include <math/vector4.hpp>


TEST(Vec4, SizeOf) {
    EXPECT_EQ(sizeof(math::vector4), sizeof(f32) * 4);
}

TEST(Vec4, XY_Coords) {
    auto v = math::vector4(1.f, 2.f, 3.f, 4.f);

    EXPECT_EQ(v.x, v.u); EXPECT_EQ(v.x, v.r);
    EXPECT_EQ(v.y, v.v); EXPECT_EQ(v.y, v.g);
    EXPECT_EQ(v.z, v.s); EXPECT_EQ(v.z, v.b);
    EXPECT_EQ(v.w, v.t); EXPECT_EQ(v.w, v.a);
}

TEST(Vec4, FromVec2) {
    auto v = math::vector2::make(1.f, 2.f);

    auto w = math::vector4(v);
    EXPECT_EQ(w, math::vector4(1.f, 2.f, 0.f, 0.f));

    auto t = math::vector4(v, 7.f, -1.f);
    EXPECT_EQ(t, math::vector4(1.f, 2.f, 7.f, -1.f));
}

TEST(Vec4, Operations) {
    auto v = math::vector4(1.f, 2.f, 3.f, 4.f);
    auto w = math::vector4(3.f, 4.f, 5.f, 6.f);

    EXPECT_EQ(-v, math::vector4(-1.f, -2.f, -3.f, -4.f));
    EXPECT_EQ(v + w, math::vector4(4.f, 6.f, 8.f, 10.f));
    EXPECT_EQ(v - w, math::vector4(-2.f, -2.f, -2.f, -2.f));
    EXPECT_EQ(2.f * v, math::vector4(2.f, 4.f, 6.f, 8.f));
    EXPECT_EQ(3.f * w, math::vector4(9.f, 12.f, 15.f, 18.f));

    v += w;
    EXPECT_EQ(v, math::vector4(4.f, 6.f, 8.f, 10.f));

    v *= 5.f;
    EXPECT_EQ(v, math::vector4(20.f, 30.f, 40.f, 50.f));

    v -= w;
    EXPECT_EQ(v, math::vector4(17.f, 26.f, 35.f, 44.f));
}

TEST(Vec4, DotProduct) {
    auto v = math::vector4(1.f, 2.f, 3.f, 4.f);
    auto w = math::vector4(3.f, 4.f, 5.f, 6.f);

    f32 dot_product = math::dot(v, w);
    EXPECT_EQ(dot_product, 50.f);
}

