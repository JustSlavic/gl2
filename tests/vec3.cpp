#include <gtest/gtest.h>

#include <math/vector2.hpp>
#include <math/vector3.hpp>


TEST(Vec3, SizeOf) {
    EXPECT_EQ(sizeof(math::vector3), sizeof(f32) * 3);
}

TEST(Vec3, XY_Coords) {
    auto v = math::vector3::make(1.f, 2.f, 3.f);

    EXPECT_EQ(v.x, v.u); EXPECT_EQ(v.x, v.r);
    EXPECT_EQ(v.y, v.v); EXPECT_EQ(v.y, v.g);
    EXPECT_EQ(v.z, v.s); EXPECT_EQ(v.z, v.b);
}

TEST(Vec3, FromVec2) {
    auto v = math::vector2::make(1.f, 2.f);

    auto t = math::vector3::make(v, 7.f);
    EXPECT_EQ(t, math::vector3::make(1.f, 2.f, 7.f));

    auto s = math::vector3::make(9.f, v);
    EXPECT_EQ(s, math::vector3::make(9.f, 1.f, 2.f));
}

TEST(Vec3, Length) {
    auto v = math::vector3::make(3.f, 4.f, 12.f);

    EXPECT_TRUE(math::equal(v.length_2(), 169.f));
    EXPECT_TRUE(math::equal(v.length(), 13.f));
    EXPECT_TRUE(math::equal(v.norm(), 13.f));
    EXPECT_TRUE(math::equal(v.normalized().norm(), 1.f));
}

TEST(Vec3, Operations) {
    auto v = math::vector3::make(1.f, 2.f, 3.f);
    auto w = math::vector3::make(3.f, 4.f, 5.f);

    EXPECT_EQ(-v, math::vector3::make(-1.f, -2.f, -3.f));
    EXPECT_EQ(v + w, math::vector3::make(4.f, 6.f, 8.f));
    EXPECT_EQ(v - w, math::vector3::make(-2.f, -2.f, -2.f));
    EXPECT_EQ(2.f * v, math::vector3::make(2.f, 4.f, 6.f));
    EXPECT_EQ(3.f * w, math::vector3::make(9.f, 12.f, 15.f));

    v += w;
    EXPECT_EQ(v, math::vector3::make(4.f, 6.f, 8.f));

    v *= 5.f;
    EXPECT_EQ(v, math::vector3::make(20.f, 30.f, 40.f));

    v -= w;
    EXPECT_EQ(v, math::vector3::make(17.f, 26.f, 35.f));
}

TEST(Vec3, DotProduct) {
    auto v = math::vector3::make(1.f, 2.f, 3.f);
    auto w = math::vector3::make(3.f, 4.f, 5.f);

    f32 dot_product = math::dot(v, w);
    EXPECT_EQ(dot_product, 26.f);
}

TEST(Vec3, CrossProduct) {
    auto v = math::vector3::make(2.f, 0.f, 0.f);
    auto w = math::vector3::make(0.f, 3.f, 0.f);
    auto u = math::vector3::make(0.f, 0.f, 6.f);

    math::vector3 cross_product = math::cross(v, w);
    EXPECT_EQ(cross_product, u);
}
