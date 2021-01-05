#include <gtest/gtest.h>

#include <math/vec2.hpp>
#include <math/vec3.hpp>


TEST(Vec3, SizeOf) {
    EXPECT_EQ(sizeof(math::vec3), sizeof(f32) * 3);
}

TEST(Vec3, XY_Coords) {
    auto v = math::vec3(1.f, 2.f, 3.f);

    EXPECT_EQ(v.x, v.u); EXPECT_EQ(v.x, v.r);
    EXPECT_EQ(v.y, v.v); EXPECT_EQ(v.y, v.g);
    EXPECT_EQ(v.z, v.s); EXPECT_EQ(v.z, v.b);
}

TEST(Vec3, FromVec2) {
    auto v = math::vec2(1.f, 2.f);

    auto w = math::vec3(v);
    EXPECT_EQ(w, math::vec3(1.f, 2.f, 0.f));

    auto t = math::vec3(v, 7.f);
    EXPECT_EQ(t, math::vec3(1.f, 2.f, 7.f));

    auto s = math::vec3(9.f, v);
    EXPECT_EQ(s, math::vec3(9.f, 1.f, 2.f));
}

TEST(Vec3, Length) {
    auto v = math::vec3(3.f, 4.f, 12.f);

    EXPECT_TRUE(math::equal(v.length_2(), 169.f));
    EXPECT_TRUE(math::equal(v.length(), 13.f));
    EXPECT_TRUE(math::equal(v.norm(), 13.f));
    EXPECT_TRUE(math::equal(v.normalized().norm(), 1.f));
}

TEST(Vec3, Operations) {
    auto v = math::vec3(1.f, 2.f, 3.f);
    auto w = math::vec3(3.f, 4.f, 5.f);

    EXPECT_EQ(-v, math::vec3(-1.f, -2.f, -3.f));
    EXPECT_EQ(v + w, math::vec3(4.f, 6.f, 8.f));
    EXPECT_EQ(v - w, math::vec3(-2.f, -2.f, -2.f));
    EXPECT_EQ(2.f * v, math::vec3(2.f, 4.f, 6.f));
    EXPECT_EQ(3.f * w, math::vec3(9.f, 12.f, 15.f));

    v += w;
    EXPECT_EQ(v, math::vec3(4.f, 6.f, 8.f));

    v *= 5.f;
    EXPECT_EQ(v, math::vec3(20.f, 30.f, 40.f));

    v -= w;
    EXPECT_EQ(v, math::vec3(17.f, 26.f, 35.f));
}

TEST(Vec3, DotProduct) {
    auto v = math::vec3(1.f, 2.f, 3.f);
    auto w = math::vec3(3.f, 4.f, 5.f);

    f32 dot_product = math::dot(v, w);
    EXPECT_EQ(dot_product, 26.f);
}

TEST(Vec3, CrossProduct) {
    auto v = math::vec3(2.f, 0.f, 0.f);
    auto w = math::vec3(0.f, 3.f, 0.f);
    auto u = math::vec3(0.f, 0.f, 6.f);

    math::vec3 cross_product = math::cross(v, w);
    EXPECT_EQ(cross_product, u);
}
