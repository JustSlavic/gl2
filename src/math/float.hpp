#pragma once

#include "consts.hpp"

#include <defines.h>
#include <cmath>
#include <type_traits>

namespace math {

inline bool equal(f32 a, f32 b) {
    return abs(b - a) < consts<f32>::eps();
}

inline bool equal(f64 a, f64 b) {
    return abs(b - a) < consts<f64>::eps();
}


inline f32 abs(f32 x) {
    // @todo make it by myself
    return ::std::abs(x);
}

inline f64 abs(f64 x) {
    // @todo make it by myself
    return ::std::abs(x);
}

inline f32 sqrt(f32 x) {
    return ::std::sqrt(x);
}

inline f64 sqrt(f64 x) {
    return ::std::sqrt(x);
}



using IEEE_754_f32_repr = union {
    struct {
#if ENDIANESS == ENDIANESS_LE
        u32 mantissa : 23;
        u32 exponent : 8;
        u32 sign : 1;
#endif
#if ENDIANESS == ENDIANESS_BE
        u32 sign : 1;
        u32 exponent : 8;
        u32 mantissa : 23;
#endif
    };

    f32 value;
};

using IEEE_754_f64_repr = union {
    struct {
#if ENDIANESS == ENDIANESS_LE
        u64 mantissa : 52;
        u64 exponent : 11;
        u64 sign : 1;
#endif
#if ENDIANESS == ENDIANESS_BE
        u64 sign : 1;
        u64 exponent : 11;
        u64 mantissa : 52;
#endif
    };

    f32 value;
};

} // math


