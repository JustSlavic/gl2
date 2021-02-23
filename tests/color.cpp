#include <gtest/gtest.h>

#include <math/color.hpp>


TEST(Color, SizeOf) {
    ASSERT_EQ(sizeof(math::color24),  sizeof(f32) * 3);
    ASSERT_EQ(sizeof(math::color32), sizeof(f32) * 4);
}

TEST(Color, Make24) {
    auto color = math::color24::make(0.2f, 0.3f, 0.4f);

    EXPECT_EQ(color.r, 0.2f);
    EXPECT_EQ(color.g, 0.3f);
    EXPECT_EQ(color.b, 0.4f);
}

TEST(Color, Make32) {
    auto color = math::color32::make(0.2f, 0.3f, 0.4f, 0.5f);

    EXPECT_EQ(color.r, 0.2f);
    EXPECT_EQ(color.g, 0.3f);
    EXPECT_EQ(color.b, 0.4f);
    EXPECT_EQ(color.a, 0.5f);
}

TEST(Color, From24bit) {
    u32 color = 0x88FCF9; // beautiful light blue

    auto color24 = math::color24::from_24bit(color);
    auto color32_1 = math::color32::from_24bit(color, 0xFF);
    auto color32_2 = math::color32::from_24bit(color, 1.f);
    
    EXPECT_EQ(color24.r, 136.f / 255.f);
    EXPECT_EQ(color24.g, 252.f / 255.f);
    EXPECT_EQ(color24.b, 249.f / 255.f);

    EXPECT_EQ(color32_1.r, 136.f / 255.f);
    EXPECT_EQ(color32_1.g, 252.f / 255.f);
    EXPECT_EQ(color32_1.b, 249.f / 255.f);
    EXPECT_EQ(color32_1.a, 1.f);
    
    EXPECT_EQ(color32_2.r, 136.f / 255.f);
    EXPECT_EQ(color32_2.g, 252.f / 255.f);
    EXPECT_EQ(color32_2.b, 249.f / 255.f);
    EXPECT_EQ(color32_2.a, 1.f);
}

TEST(Color, From32bit) {
    u32 color = 0x88FCF9FF; // beautiful light blue

    auto color24 = math::color24::from_32bit(color);
    auto color32 = math::color32::from_32bit(color);

    EXPECT_EQ(color24.r, 136.f / 255.f);
    EXPECT_EQ(color24.g, 252.f / 255.f);
    EXPECT_EQ(color24.b, 249.f / 255.f);

    EXPECT_EQ(color32.r, 136.f / 255.f);
    EXPECT_EQ(color32.g, 252.f / 255.f);
    EXPECT_EQ(color32.b, 249.f / 255.f);
    EXPECT_EQ(color32.a, 1.f);
}

TEST(Color, Subcolor24) {
    u32 color = 0x88FCF9FF; // beautiful light blue

    auto color32 = math::color32::from_32bit(color);
    auto color24 = math::color24::from_32bit(color);
    ASSERT_EQ(color32.rgb, color24);
}

TEST(Color, To24bit) {
    u32 color = 0x88FCF9; // beautiful light blue

    auto color24 = math::color24::make(136.f / 255.f, 252.f / 255.f, 249.f / 255.f);
    EXPECT_EQ(color24.to_24bit(), color);

    auto color32 = math::color32::make(136.f / 255.f, 252.f / 255.f, 249.f / 255.f, 1.f);
    EXPECT_EQ(color32.to_24bit(), color);
}

TEST(Color, To32bit) {
    u32 color = 0x88FCF9FF; // beautiful light blue

    auto color24_1 = math::color24::make(136.f / 255.f, 252.f / 255.f, 249.f / 255.f);
    EXPECT_EQ(color24_1.to_32bit(1.f), color);

    auto color24_2 = math::color24::make(136.f / 255.f, 252.f / 255.f, 249.f / 255.f);
    EXPECT_EQ(color24_2.to_32bit(0xFF), color);

    auto color32 = math::color32::make(136.f / 255.f, 252.f / 255.f, 249.f / 255.f, 1.f);
    EXPECT_EQ(color32.to_32bit(), color);
}

TEST(Color, Equivalency24) {
    u32 color = 0x88FCF9; // beautiful light blue

    auto color24 = math::color24::from_24bit(color);
    EXPECT_EQ(color24.to_24bit(), color);

    auto color32_1 = math::color32::from_24bit(color, 1.f);
    EXPECT_EQ(color32_1.to_24bit(), color);

    auto color32_2 = math::color32::from_24bit(color, 0xFF);
    EXPECT_EQ(color32_2.to_24bit(), color);
}

TEST(Color, Equivalency32) {
    u32 color = 0x88FCF9FF; // beautiful light blue

    auto color24_1 = math::color24::from_32bit(color);
    EXPECT_EQ(color24_1.to_32bit(1.f), color);

    auto color24_2 = math::color24::from_32bit(color);
    EXPECT_EQ(color24_2.to_32bit(0xFF), color);

    auto color32 = math::color32::from_32bit(color);
    EXPECT_EQ(color32.to_32bit(), color);
}






