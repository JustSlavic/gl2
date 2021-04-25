#include <gtest/gtest.h>

#include <math/vector2.hpp>
#include <math/vector3.hpp>


TEST(Vector2, SizeOf) {
    EXPECT_EQ(sizeof(math::vector2), sizeof(f32) * 2);
}

TEST(Vector2, XY_Coords) {
    auto v = math::vector2::make(1.f, 2.f);

    EXPECT_EQ(v.x, v.u);
    EXPECT_EQ(v.y, v.v);
}

TEST(Vector2, FromVec3) {
    auto v = math::vector3::make(9.f, 2.f, 5.f);
    auto w = v.xy;

    EXPECT_EQ(w, math::vector2::make(9.f, 2.f));
}

TEST(Vector2, Length) {
    auto v = math::vector2::make(3.f, 4.f);

    EXPECT_TRUE(math::equal(v.length_2(), 25.f));
    EXPECT_TRUE(math::equal(v.length(), 5.f));
    EXPECT_TRUE(math::equal(v.norm(), 5.f));
    EXPECT_TRUE(math::equal(v.normalized().norm(), 1.f));
}

TEST(Vector2, Operations) {
    auto v = math::vector2::make(1.f, 2.f);
    auto w = math::vector2::make(3.f, 4.f);

    EXPECT_EQ(-v, math::vector2::make(-1.f, -2.f));
    EXPECT_EQ(v + w, math::vector2::make(4.f, 6.f));
    EXPECT_EQ(v - w, math::vector2::make(-2.f, -2.f));
    EXPECT_EQ(2.f * v, math::vector2::make(2.f, 4.f));
    EXPECT_EQ(3.f * w, math::vector2::make(9.f, 12.f));

    v += w;
    EXPECT_EQ(v, math::vector2::make(4.f, 6.f));

    v *= 5.f;
    EXPECT_EQ(v, math::vector2::make(20.f, 30.f));

    v -= w;
    EXPECT_EQ(v, math::vector2::make(17.f, 26.f));
}

TEST(Vector2, DotProduct) {
    auto v = math::vector2::make(1.f, 2.f);
    auto w = math::vector2::make(3.f, 4.f);

    f32 product = math::dot(v, w);
    EXPECT_EQ(product, 11.f);
}

