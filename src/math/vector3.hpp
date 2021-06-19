#pragma once

#include <defines.hpp>
#include "float.hpp"
#include "color.hpp"
#include "vector2.hpp"


namespace math {


struct vector3 {
    union {
        struct { f32  x,  y,  z; };
        struct { f32  u,  v,  s; };
        struct { f32  r,  g,  b; };
        struct { f32 _1, _2, _3; };

        struct { vector2  xy; f32 dummy_z; };
        struct { vector2  uv; f32 dummy_s; };
        struct { vector2  rg; f32 dummy_b; };
        struct { vector2 _12; f32 dummy_3; };

        color24 rgb;
    };

    inline static vector3 make () { return { 0.f, 0.f, 0.f }; }
    inline static vector3 make (f32 value) { return { value, value, value }; }
    inline static vector3 make (f32 x, f32 y, f32 z) { return { x, y, z }; }
    inline static vector3 make (f32 x, vector2 v2) { return { x, v2.x, v2.y }; }
    inline static vector3 make (vector2 v2, f32 z) { return { v2.x, v2.y, z }; }

    inline f32  length_2   () const { return x*x + y*y + z*z; } // length squared
    inline f32  length     () const { return math::sqrt(length_2()); }
    inline f32  norm       () const { return length(); }
    inline vector3 normalized () const { return { x / norm(), y / norm(), z / norm() }; }

    inline vector3& operator += (const vector3& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    inline vector3& operator -= (const vector3& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    inline vector3& operator *= (f32 c) {
        x *= c;
        y *= c;
        z *= c;
        return *this;
    }
};


inline f32 dot (const vector3& a, const vector3& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline vector3 cross (const vector3& a, const vector3& b) {
    return {
        a.y*b.z - a.z*b.y,
        a.x*b.z - a.z*b.x,
        a.x*b.y - a.y*b.x
    };
}

inline bool equal (const vector3& a, const vector3& b) {
    return math::equal(a.x, b.x) && math::equal(a.y, b.y) && math::equal(a.z, b.z);
}

inline vector3 operator -  (const vector3& a) {
    return { -a.x, -a.y, -a.z };
}

inline vector3 operator +  (const vector3& a, const vector3& b) {
    return { a.x + b.x, a.y + b.y, a.z + b.z };
}

inline vector3 operator -  (const vector3& a, const vector3& b) {
    return { a.x - b.x, a.y - b.y, a.z - b.z };
}

inline vector3 operator *  (const vector3& a, f32 c) {
    return { c * a.x, c * a.y, c * a.z };
}

inline vector3 operator *  (f32 c, const vector3& a) {
    return { c * a.x, c * a.y, c * a.z };
}

inline vector3 operator /  (const vector3& a, f32 denom) {
    return { a.x / denom, a.y / denom, a.z / denom };
}

inline bool operator == (const vector3& a, const vector3& b) {
    return (a.x == b.x) && (a.y == b.y) && (a.z == b.z);
}

inline bool operator != (const vector3& a, const vector3& b) {
    return !(a == b);
}

inline vector3 lerp (const vector3& a, const vector3& b, f32 t) {
    return { lerp(a.x, b.x, t), lerp(a.y, b.y, t), lerp(a.z, b.z, t) };
}


math::vector3 intersect_plane(math::vector3 ray_origin, math::vector3 ray_direction, math::vector3 point_on_plane, math::vector3 plane_normal);
math::vector3 intersect_z0_plane(math::vector3 ray_start, math::vector3 vector);


} // math
