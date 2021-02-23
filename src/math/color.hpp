#pragma once

#include <defines.h>

namespace math {

struct color24 {
    struct { f32 r; f32 g; f32 b; };

    static color24 make(f32 v);
    static color24 make(f32 r, f32 g, f32 b);

    static color24 from_24bit (u32 color_24bit);
    static color24 from_32bit (u32 color_32bit);

    u32  to_24bit () const;
    u32  to_32bit (f32 alpha) const;
    u32  to_32bit (i32 alpha) const;
    u32  to_32bit (u32 alpha) const;
};

inline bool operator == (const color24& lhs, const color24& rhs) {
    return (lhs.r == rhs.r) && (lhs.g == rhs.g) && (lhs.b == rhs.b);
}


struct color32 {
    struct {
        union {
            struct { f32 r, g, b; };
            color24 rgb;
        };
        f32 a;
    };

    static color32 make(f32 r, f32 g, f32 b, f32 a);

    static color32 from_24bit (u32 color_24bit, f32 alpha);
    static color32 from_24bit (u32 color_24bit, i32 alpha);
    static color32 from_24bit (u32 color_24bit, u32 alpha);
    static color32 from_32bit (u32 color_32bit);

    u32  to_24bit () const;
    u32  to_32bit () const;
};

inline bool operator == (const color32& lhs, const color32& rhs) {
    return (lhs.r == rhs.r) && (lhs.g == rhs.g) && (lhs.b == rhs.b) && (lhs.a == rhs.a);
}

using color = color32;

} // math
