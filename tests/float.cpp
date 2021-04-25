#include <gtest/gtest.h>

#include <math/float.hpp>
#include <utils.h>


TEST(Float, Epsilon) {
    math::IEEE_754_f32_repr repr;
    repr.sign = 0x0;
    repr.exponent = 104; // @todo: why exactly 104?
    repr.mantissa = 0x0;

    EXPECT_EQ(math::consts<f32>::eps(), repr.value);
}

TEST(Float, Abs) {
    EXPECT_EQ(1.f, math::abs(-1.f));
}

TEST(Float, Equal) {
    f32 a = .211f + .3f;
    f32 b = .511f;

    EXPECT_NE(0.3f, 0.f);
    EXPECT_NE(a, b);
    EXPECT_TRUE(math::equal(a, b));
}

TEST(Float, NormalMinimum) {
    math::IEEE_754_f32_repr repr;
    repr.sign = 0x0;
    repr.exponent = 0x1;
    repr.mantissa = 0x0;

    // print_binary_repr(repr.value);

    EXPECT_EQ(math::consts<f32>::min(), repr.value);
}

TEST(Float, NormalMaximum) {
    math::IEEE_754_f32_repr repr;
    repr.sign = 0x0;
    repr.exponent = 0xfe;
    repr.mantissa = 0x7fffff;

    // print_binary_repr(repr.value);

    EXPECT_EQ(math::consts<f32>::max(), repr.value);
}


TEST(Float, PositiveInfinity) {
    math::IEEE_754_f32_repr repr;
    repr.sign = 0x0;
    repr.exponent = 0xff;
    repr.mantissa = 0x0;

    // print_binary_repr(repr.value);

    EXPECT_EQ(math::consts<f32>::inf(), repr.value);
}

TEST(Float, NegativeInfinity) {
    math::IEEE_754_f32_repr repr;
    repr.sign = 0x1;
    repr.exponent = 0xff;
    repr.mantissa = 0x0;

    // print_binary_repr(repr.value);

    EXPECT_EQ(-math::consts<f32>::inf(), repr.value);   
}

TEST(Float, NaN) {
    math::IEEE_754_f32_repr repr;
    repr.sign = 0x0;
    repr.exponent = 0xff;
    repr.mantissa = 0x400000;

    // NaN != NaN
    EXPECT_NE(math::consts<f32>::nan(), repr.value);

    using to_decimal = union {
        f32 f;
        u32 d;
    };
    
    to_decimal a;
    to_decimal b;

    a.f = repr.value;
    b.f = math::consts<f32>::nan();

    // print_binary_repr(repr.value);
    // print_binary_repr(math::consts<f32>::nan());

    // But they binary representation is really the same
    EXPECT_EQ(a.d, b.d);

}

TEST(Float, Lerp) {
    EXPECT_EQ(math::lerp(5.f, 9.f, 0.f), 5.f);
    EXPECT_EQ(math::lerp(5.f, 9.f, 1.f), 9.f);
    EXPECT_EQ(math::lerp(5.f, 9.f, .5f), 7.f);
}
