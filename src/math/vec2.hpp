#pragma once

#include <defines.h>
#include "float.hpp"


namespace math {

struct vec3;
struct vec2 {
    union {
        struct { f32 x; f32 y; };
        struct { f32 u; f32 v; };
    };

             vec2 ();
    explicit vec2 (f32 v);
             vec2 (f32 x, f32 y);
    explicit vec2 (const vec3& v);

    inline f32  length_2   () const { return x*x + y*y; }
    inline f32  length     () const { return math::sqrt(length_2()); }
    inline f32  norm       () const { return length(); }
    inline vec2 normalized () const { return vec2(x / norm(), y / norm()); }

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

    inline vec2& operator *= (f32 c) {
        x *= c;
        y *= c;
        return *this;
    }
};


inline bool equal (const vec2& a, const vec2& b) {
    return math::equal(a.x, b.x) && math::equal(a.y, b.y);
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

inline f32 dot (const vec2& a, const vec2& b) {
    return a.x * b.x + a.y * b.y;
}

// @think is there a sense to do such an operation?
inline vec2 abs (const vec2& a) {
    return vec2(abs(a.x), abs(a.y));
}

inline vec2 sqrt(const vec2& a) {
    return vec2(sqrt(a.x), sqrt(a.y));
}

} // math
