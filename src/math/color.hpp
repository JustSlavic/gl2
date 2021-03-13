#pragma once

#include <defines.h>

namespace math {

struct color24 {
    struct { f32 r; f32 g; f32 b; };

    static color24 make(f32 v); // value in [0, 1]
    static color24 make(i32 v); // value in [0, 255]
    static color24 make(u32 v); // value in [0, 255]
    static color24 make(f32 r, f32 g, f32 b); // values are in [0, 1]
    static color24 make(i32 r, i32 g, i32 b); // values are in [0, 255]
    static color24 make(u32 r, u32 g, u32 b); // values are in [0, 255]

    static color24 unpack (u32 packed_color); // Bit pattern in packed color: ARGB

    u32  pack () const;
    u32  pack_32bit (f32 alpha = 1.f) const; // alpha in [0, 1]
    u32  pack_32bit (i32 alpha = 255) const; // alpha in [0, 255]
    u32  pack_32bit (u32 alpha = 255) const; // alpha in [0, 255]
};


struct color32 {
    struct {
        union {
            struct { f32 r, g, b; };
            color24 rgb;
        };
        f32 a;
    };

    static color32 make();
    static color32 make(f32 r, f32 g, f32 b, f32 a = 1.f);
    static color32 make(i32 r, i32 g, i32 b, i32 a = 255);
    static color32 make(u32 r, u32 g, u32 b, u32 a = 255);

    static color32 unpack (u32 packed_color);
    static color32 unpack_24bit (u32 color_24bit, f32 alpha); // alpha in [0, 1]
    static color32 unpack_24bit (u32 color_24bit, i32 alpha); // alpha in [0, 255]
    static color32 unpack_24bit (u32 color_24bit, u32 alpha); // alpha in [0, 255]

    u32  pack () const;
};

using color = color32;


inline bool operator == (const color24& lhs, const color24& rhs) {
    return (lhs.r == rhs.r) && (lhs.g == rhs.g) && (lhs.b == rhs.b);
}


inline bool operator != (const color24& lhs, const color24& rhs) {
    return !(lhs == rhs);
}


inline bool operator == (const color32& lhs, const color32& rhs) {
    return (lhs.r == rhs.r) && (lhs.g == rhs.g) && (lhs.b == rhs.b) && (lhs.a == rhs.a);
}


inline bool operator != (const color32& lhs, const color32& rhs) {
    return !(lhs == rhs);
}


} // math
