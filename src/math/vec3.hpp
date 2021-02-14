#pragma once

#include <defines.h>
#include "float.hpp"

namespace math {

struct vec2;
struct vec3 {
    union {
        struct { f32  x,  y,  z; };
        struct { f32  u,  v,  s; };
        struct { f32  r,  g,  b; };
        struct { f32 _1, _2, _3; };
    };

             vec3 ();
    explicit vec3 (f32 v);
             vec3 (f32 x, f32 y, f32 z);
    explicit vec3 (const vec2& v);
             vec3 (const vec2& v, f32 z);
             vec3 (f32 x, const vec2& v);

    inline f32  length_2   () const { return x*x + y*y + z*z; } // length squared
    inline f32  length     () const { return math::sqrt(length_2()); }
    inline f32  norm       () const { return length(); }
    inline vec3 normalized () const { return vec3(x / norm(), y / norm(), z / norm()); }

    inline vec3& operator += (const vec3& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    inline vec3& operator -= (const vec3& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    inline vec3& operator *= (f32 c) {
        x *= c;
        y *= c;
        z *= c;
        return *this;
    }
};


inline f32 dot (const vec3& a, const vec3& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline vec3 cross (const vec3& a, const vec3& b) {
    return vec3(
        a.y*b.z - a.z*b.y,
        a.x*b.z - a.z*b.x,
        a.x*b.y - a.y*b.x
        );
}

inline bool equal (const vec3& a, const vec3& b) {
    return math::equal(a.x, b.x) && math::equal(a.y, b.y) && math::equal(a.z, b.z);
}

inline vec3 operator -  (const vec3& a) {
    return vec3(-a.x, -a.y, -a.z);
}

inline vec3 operator +  (const vec3& a, const vec3& b) {
    return vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

inline vec3 operator -  (const vec3& a, const vec3& b) {
    return vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

inline vec3 operator *  (const vec3& a, f32 c) {
    return vec3(c * a.x, c * a.y, c * a.z);
}

inline vec3 operator *  (f32 c, const vec3& a) {
    return vec3(c * a.x, c * a.y, c * a.z);
}

inline vec3 operator /  (const vec3& a, f32 denom) {
    return vec3(a.x / denom, a.y / denom, a.z / denom);
}

inline bool operator == (const vec3& a, const vec3& b) {
    return (a.x == b.x) && (a.y == b.y) && (a.z == b.z);
}

inline bool operator != (const vec3& a, const vec3& b) {
    return !(a == b);
}

} // math
