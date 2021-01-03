#include <gtest/gtest.h>

#include <math/vec2.hpp>


TEST(Vec2, SizeOf) {
    EXPECT_EQ(sizeof(math::vec2), sizeof(f32) * 2);
}

TEST(Vec2, XY_Coords) {
    auto v = math::vec2(1.f, 2.f);

    EXPECT_EQ(v.x, v.u);
    EXPECT_EQ(v.y, v.v);
}

TEST(Vec2, Epsilon) {
    auto v = math::consts<math::vec2>::eps();

    EXPECT_EQ(v.x, math::consts<f32>::eps());
    EXPECT_EQ(v.y, math::consts<f32>::eps());
}

TEST(Vec2, Length) {
    auto v = math::vec2(3.f, 4.f);

    EXPECT_TRUE(math::equal(v.length_2(), 25.f));
    EXPECT_TRUE(math::equal(v.length(), 5.f));
    EXPECT_TRUE(math::equal(v.norm(), 5.f));
    EXPECT_TRUE(math::equal(v.normalized().norm(), 1.f));
}

TEST(Vec2, Operations) {
    auto v = math::vec2(1.f, 2.f);
    auto w = math::vec2(3.f, 4.f);

    f32 product = math::dot(v, w);
    EXPECT_EQ(product, 11.f);
    EXPECT_EQ(-v, math::vec2(-1.f, -2.f));
    EXPECT_EQ(v + w, math::vec2(4.f, 6.f));
    EXPECT_EQ(v - w, math::vec2(-2.f, -2.f));
    EXPECT_EQ(2.f * v, math::vec2(2.f, 4.f));
    EXPECT_EQ(3.f * w, math::vec2(9.f, 12.f));
}
