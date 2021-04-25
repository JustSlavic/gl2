#pragma once

#include <defines.h>
#include "float.hpp"
#include "color.hpp"
#include "vector2.hpp"
#include "vector3.hpp"


namespace math {


struct vector4 {
    union {
        struct { f32 x; f32 y; f32 z; f32 w; };
        struct { f32 u; f32 v; f32 s; f32 t; };
        struct { f32 r; f32 g; f32 b; f32 a; };

        struct { vector2 xy, zw; };
        struct { vector2 uv, st; };

        struct { vector3 xyz; f32 dummy_w; };
        struct { color24 rgb; f32 dummy_a; };

        color32 rgba;
    };

    inline static vector4 make () { return { 0.f, 0.f, 0.f, 0.f }; }
    inline static vector4 make (f32 value) { return { value, value, value, value }; }
    inline static vector4 make (f32 x, f32 y, f32 z, f32 w) { return { x, y, z, w }; }

    inline static vector4 make (f32 x, f32 y, vector2 v2) { return { x, y, v2.x, v2.y }; }
    inline static vector4 make (vector2 v1, f32 z, f32 w) { return { v1.x, v1.y, z, w }; }
    inline static vector4 make (f32 x, vector2 v1, f32 w) { return { x, v1.x, v1.y, w }; }
    inline static vector4 make (vector2 v1, vector2 v2) { return { v1.x, v1.y, v2.x, v2.y }; }

    inline static vector4 make (f32 x, vector3 v3) { return { x, v3.x, v3.y, v3.z }; }
    inline static vector4 make (vector3 v3, f32 w) { return { v3.x, v3.y, v3.z, w }; }

    inline vector4& operator += (const vector4& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        w += other.w;
        return *this;
    }

    inline vector4& operator -= (const vector4& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        w -= other.w;
        return *this;
    }

    inline vector4& operator *= (f32 c) {
        x *= c;
        y *= c;
        z *= c;
        w *= c;
        return *this;
    }
};

inline f32 dot (const vector4& a, const vector4& b) {
    return a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w;
}

inline bool equal (const vector4& a, const vector4& b) {
    return math::equal(a.x, b.x) && math::equal(a.y, b.y) && math::equal(a.z, b.z) && math::equal(a.w, b.w);
}

inline vector4 operator -  (const vector4& a) {
    return { -a.x, -a.y, -a.z, -a.w };
}

inline vector4 operator +  (const vector4& a, const vector4& b) {
    return { a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w };
}

inline vector4 operator -  (const vector4& a, const vector4& b) {
    return { a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w };
}

inline vector4 operator *  (const vector4& a, f32 c) {
    return { c * a.x, c * a.y, c * a.z, c * a.w };
}

inline vector4 operator *  (f32 c, const vector4& a) {
    return { c * a.x, c * a.y, c * a.z, c * a.w };
}

inline bool operator == (const vector4& a, const vector4& b) {
    return (a.x == b.x) && (a.y == b.y) && (a.z == b.z) && (a.w == b.w);
}

inline bool operator != (const vector4& a, const vector4& b) {
    return !(a == b);
}

inline vector4 lerp (const vector4& a, const vector4& b, f32 t) {
    return { lerp(a.x, b.x, t), lerp(a.y, b.y, t), lerp(a.z, b.z, t), lerp(a.w, b.w, t) };
}

} // math
