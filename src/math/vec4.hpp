#pragma once

#include <defines.h>
#include "float.hpp"

namespace math {

struct vec2;
struct vec3;
struct vec4 {
    union {
        struct { f32 x; f32 y; f32 z; f32 w; };
        struct { f32 u; f32 v; f32 s; f32 t; };
        struct { f32 r; f32 g; f32 b; f32 a; };
    };

             vec4 ();
    explicit vec4 (f32 v);
             vec4 (f32 x, f32 y, f32 z, f32 w);
    explicit vec4 (const vec2& v);
             vec4 (const vec2& v, f32 z, f32 w);
             // vec4(f32, vec2, f32);
             // vec4(f32, f32, vec2);
             vec4 (const vec2& v, const vec2& w);
    explicit vec4 (const vec3& v);
             vec4 (const vec3& v, f32 w);
             // vec4(f32, vec3);

    inline vec4& operator += (const vec4& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        w += other.w;
        return *this;
    }

    inline vec4& operator -= (const vec4& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        w -= other.w;
        return *this;
    }

    inline vec4& operator *= (f32 c) {
        x *= c;
        y *= c;
        z *= c;
        w *= c;
        return *this;
    }
};

inline f32 dot (const vec4& a, const vec4& b) {
    return a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w;
}

inline bool equal (const vec4& a, const vec4& b) {
    return math::equal(a.x, b.x) && math::equal(a.y, b.y) && math::equal(a.z, b.z) && math::equal(a.w, b.w);
}

inline vec4 operator -  (const vec4& a) {
    return vec4(-a.x, -a.y, -a.z, -a.w);
}

inline vec4 operator +  (const vec4& a, const vec4& b) {
    return vec4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

inline vec4 operator -  (const vec4& a, const vec4& b) {
    return vec4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

inline vec4 operator *  (const vec4& a, f32 c) {
    return vec4(c * a.x, c * a.y, c * a.z, c * a.w);
}

inline vec4 operator *  (f32 c, const vec4& a) {
    return vec4(c * a.x, c * a.y, c * a.z, c * a.w);
}

inline bool operator == (const vec4& a, const vec4& b) {
    return (a.x == b.x) && (a.y == b.y) && (a.z == b.z) && (a.w == b.w);
}

inline bool operator != (const vec4& a, const vec4& b) {
    return !(a == b);
}

} // math
