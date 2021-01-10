#pragma once

#include <defines.h>

namespace math {

struct color_rgb {
    struct { f32 r; f32 g; f32 b; };

    static color_rgb make(f32 r, f32 g, f32 b);

    static color_rgb from_24bit (u32 color_24bit);
    static color_rgb from_32bit (u32 color_32bit);

    u32  to_24bit () const;
    u32  to_32bit (f32 alpha) const;
    u32  to_32bit (i32 alpha) const;
    u32  to_32bit (u32 alpha) const;
};

inline bool operator == (const color_rgb& lhs, const color_rgb& rhs) {
    return (lhs.r == rhs.r) && (lhs.g == rhs.g) && (lhs.b == rhs.b);
}


struct color_rgba {
    struct {
        union {
            struct { f32 r; f32 g; f32 b; };
            color_rgb rgb;
        };
        f32 a;
    };

    static color_rgba make(f32 r, f32 g, f32 b, f32 a);

    static color_rgba from_24bit (u32 color_24bit, f32 alpha);
    static color_rgba from_24bit (u32 color_24bit, i32 alpha);
    static color_rgba from_24bit (u32 color_24bit, u32 alpha);
    static color_rgba from_32bit (u32 color_32bit);

    u32  to_24bit () const;
    u32  to_32bit () const;
};

inline bool operator == (const color_rgba& lhs, const color_rgba& rhs) {
    return (lhs.r == rhs.r) && (lhs.g == rhs.g) && (lhs.b == rhs.b) && (lhs.a == rhs.a);
}

using color_24 = color_rgb;
using color_32 = color_rgba;
using color    = color_rgba;

} // math
