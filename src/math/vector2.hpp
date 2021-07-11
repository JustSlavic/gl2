#pragma once

#include <stdio.h>
#include <defines.hpp>
#include "float.hpp"


namespace math {


enum class aspect {
    column,
    row
};


struct vector2 {
    union {
        struct { f32  x,  y; };
        struct { f32  u,  v; };
        struct { f32 _1, _2; };
        f32 at[2];
    };

    static vector2 zero;
    static vector2 one;
    static vector2 one_x;
    static vector2 one_y;

    inline static vector2 make () { return { 0, 0 }; }
    inline static vector2 make (f32 value) { return { value, value }; }
    inline static vector2 make (f32 x, f32 y) { return { x, y }; }

    inline float32 length_2   () const { return x*x + y*y; }
    inline float32 length     () const { return math::sqrt(length_2()); }
    inline float32 norm       () const { return length(); }
    inline vector2 normalized () const { auto n = norm(); return { x / n, y / n }; }

    inline vector2& operator += (const vector2& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    inline vector2& operator -= (const vector2& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    inline vector2& operator += (float32 constant) {
        x += constant;
        y += constant;
        return *this;
    }

    inline vector2& operator -= (float32 constant) {
        x -= constant;
        y -= constant;
        return *this;
    }

    inline vector2& operator *= (f32 c) {
        x *= c;
        y *= c;
        return *this;
    }
};


inline f32 length (const vector2& a) {
    return a.length();
}

inline vector2 normalize (const vector2& a) {
    return a.normalized();
}

inline bool equal (const vector2& a, const vector2& b) {
    return math::equal(a.x, b.x) && math::equal(a.y, b.y);
}

inline vector2 operator -  (const vector2& a) {
    return { -a.x, -a.y };
}

inline vector2 operator +  (const vector2& a, const vector2& b) {
    return { a.x + b.x, a.y + b.y };
}

inline vector2 operator -  (const vector2& a, const vector2& b) {
    return { a.x - b.x, a.y - b.y };
}

inline vector2 operator *  (const vector2& a, f32 c) {
    return { c * a.x, c * a.y };
}

inline vector2 operator *  (f32 c, const vector2& a) {
    return { c * a.x, c * a.y };
}

inline vector2 operator /  (const vector2& a, f32 c) {
    return { a.x / c, a.y / c };
}

inline bool operator == (const vector2& a, const vector2& b) {
    return (a.x == b.x) && (a.y == b.y);
}

inline bool operator != (const vector2& a, const vector2& b) {
    return !(a == b);
}

inline f32 dot (const vector2& a, const vector2& b) {
    return a.x * b.x + a.y * b.y;
}

inline vector2 lerp (vector2 a, vector2 b, f32 t) {
    return { lerp(a.x, b.x, t), lerp(a.y, b.y, t) };
}

// Reflect 2D-vector from the plane with the normal n
//
// v\  /r | n
//   \/   |
// --------
//
inline vector2 reflect(vector2 v, vector2 n) {
    return v - 2 * dot(v, n) * n;
}

// Refract 2D-vector through the plane with the normal n and ratio of indices of refraction eta
//
// v\    | n
//   \   |
// --------
//    |
//    |r
//
// Exact documentation see at
// https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/refract.xhtml
inline vector2 refract (vector2 v, vector2 n, f32 eta) {
    f32 k = 1 - eta * eta * (1 - dot(n, v) * dot(n, v));
    if (k < 0) {
        return vector2::zero;
    }
    
    return eta * v - (eta * dot(n, v) + sqrt(k)) * n;
}




template <aspect A = aspect::column>
void print(const vector2& v) {
    if constexpr (A == aspect::column) {
        printf("[%f]\n[%f]\n", v.x, v.y);
        return;
    }

    if constexpr (A == aspect::row) {
        printf("[%f, %f]\n", v.x, v.y);
    }
}

} // math
