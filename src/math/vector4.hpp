#pragma once

#include <defines.h>
#include "float.hpp"

namespace math {

struct vector2;
struct vector3;
struct vector4 {
    union {
        struct { f32 x; f32 y; f32 z; f32 w; };
        struct { f32 u; f32 v; f32 s; f32 t; };
        struct { f32 r; f32 g; f32 b; f32 a; };
    };

             vector4 ();
    explicit vector4 (f32 v);
             vector4 (f32 x, f32 y, f32 z, f32 w);
    explicit vector4 (const vector2& v);
             vector4 (const vector2& v, f32 z, f32 w);
             // vector4(f32, vector2, f32);
             // vector4(f32, f32, vector2);
             vector4 (const vector2& v, const vector2& w);
    explicit vector4 (const vector3& v);
             vector4 (const vector3& v, f32 w);
             // vector4(f32, vector3);

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
    return vector4(-a.x, -a.y, -a.z, -a.w);
}

inline vector4 operator +  (const vector4& a, const vector4& b) {
    return vector4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

inline vector4 operator -  (const vector4& a, const vector4& b) {
    return vector4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

inline vector4 operator *  (const vector4& a, f32 c) {
    return vector4(c * a.x, c * a.y, c * a.z, c * a.w);
}

inline vector4 operator *  (f32 c, const vector4& a) {
    return vector4(c * a.x, c * a.y, c * a.z, c * a.w);
}

inline bool operator == (const vector4& a, const vector4& b) {
    return (a.x == b.x) && (a.y == b.y) && (a.z == b.z) && (a.w == b.w);
}

inline bool operator != (const vector4& a, const vector4& b) {
    return !(a == b);
}

} // math
