#pragma once

#include "consts.hpp"

#include <defines.h>
#include <type_traits>

namespace math {

inline f32 abs(f32 x) {
    return ::std::abs(x);
}

inline f64 abs(f64 x) {
    return ::std::abs(x);
}

inline bool equal(f32 a, f32 b) {
    return ::math::abs(b - a) < consts<f32>::eps();
}

inline bool equal(f64 a, f64 b) {
    return ::math::abs(b - a) < consts<f64>::eps();
}

inline f32 radians(f32 a) {
    return a * consts<f32>::pi() / 180.f;
}

inline f64 radians(f64 a) {
    return a * consts<f64>::pi() / 180.0;
}

inline f32 angles(f32 a) {
    return a * 180.f / consts<f32>::pi();
}

inline f64 angles(f64 a) {
    return a * 180.0 / consts<f64>::pi();
}

// @todo make all operations below by myself
inline bool is_nan(f32 a) {
    return ::std::isnan(a);
}

inline bool is_nan(f64 a) {
    return ::std::isnan(a);
}


inline f32 sqrt(f32 x) {
    return ::std::sqrt(x);
}

inline f64 sqrt(f64 x) {
    return ::std::sqrt(x);
}

// =======================

// @Warning! This works only on LITTLE ENDIAN setups
using IEEE_754_f32_repr = union {
    struct {
        u32 mantissa : 23;
        u32 exponent : 8;
        u32 sign : 1;
    };

    f32 value;
};

using IEEE_754_f64_repr = union {
    struct {
        u64 mantissa : 52;
        u64 exponent : 11;
        u64 sign : 1;
    };

    f32 value;
};

} // math


