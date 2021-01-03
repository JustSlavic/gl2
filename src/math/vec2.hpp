#pragma once

#include <defines.h>
#include "float.hpp"


namespace math {

struct vec2 {
    union {
        struct { f32 x; f32 y; };
        struct { f32 u; f32 v; };
    };

    constexpr          vec2()             :x(0.f), y(0.f) {}
    constexpr explicit vec2(f32 v)        :x(v),   y(v)   {}
    constexpr          vec2(f32 x, f32 y) :x(x),   y(y)   {}

    inline f32  length_2() const { return x*x + y*y; }
    inline f32  length()   const { return math::sqrt(length_2()); }
    inline f32  norm()     const { return length(); }
    inline vec2 normalized()     { return vec2(x / norm(), y / norm()); }

    inline vec2& operator += (const vec2& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    inline vec2& operator -= (const vec2& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }
};


template <>
struct consts<vec2> {
    using self = vec2;

    static constexpr self eps() noexcept { return vec2(consts<f32>::eps(), consts<f32>::eps()); }
    static constexpr self min() noexcept { return vec2(consts<f32>::min(), consts<f32>::min()); }
    static constexpr self max() noexcept { return vec2(consts<f32>::max(), consts<f32>::max()); }
    static constexpr self inf() noexcept { return vec2(consts<f32>::inf(), consts<f32>::inf()); }
};

inline bool equal (const vec2& a, const vec2& b) {
    return math::equal(a.x, b.x) && math::equal(a.y, b.y);
}

inline f32 dot (const vec2& a, const vec2& b) {
    return a.x * b.x + a.y * b.y;
}

inline vec2 operator -  (const vec2& a) {
    return vec2(-a.x, -a.y);
}

inline vec2 operator +  (const vec2& a, const vec2& b) {
    return vec2(a.x + b.x, a.y + b.y);
}

inline vec2 operator -  (const vec2& a, const vec2& b) {
    return vec2(a.x - b.x, a.y - b.y);
}

inline vec2 operator *  (const vec2& a, f32 c) {
    return vec2(c * a.x, c * a.y);
}

inline vec2 operator *  (f32 c, const vec2& a) {
    return vec2(c * a.x, c * a.y);
}

inline bool operator == (const vec2& a, const vec2& b) {
    return (a.x == b.x) && (a.y == b.y);
}

inline bool operator != (const vec2& a, const vec2& b) {
    return !(a == b);
}

} // math
