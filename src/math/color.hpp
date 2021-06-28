#pragma once

#include <defines.hpp>
#include "float.hpp"

namespace math {


enum class color_layout {
    ARGB,
    RGBA,
    RGB,
};


template <color_layout L>
struct packed_color {
    u32 value;

    explicit packed_color (u32 v) : value(v) {}
    u32 get_value () const { return value; }
};


struct color24 {
    struct { f32 r; f32 g; f32 b; };

    static color24 make (f32 v); // value in [0, 1]
    static color24 make (i32 v); // value in [0, 255]
    static color24 make (u32 v); // value in [0, 255]
    static color24 make (f32 r, f32 g, f32 b); // values in [0, 1]
    static color24 make (i32 r, i32 g, i32 b); // values in [0, 255]
    static color24 make (u32 r, u32 g, u32 b); // values in [0, 255]

    template <color_layout L>
    static color24 unpack (packed_color<L>) = delete;

    template <color_layout L>
    packed_color<L> pack () const = delete;

    template <color_layout L, typename T>
    packed_color<L> pack_32bit (T) const = delete;

    static color24 black;
    static color24 white;
    static color24 red;
    static color24 green;
    static color24 blue;
};

template <> color24 color24::unpack (packed_color<color_layout::RGB> color_);
template <> packed_color<color_layout::RGB>  color24::pack () const;
template <> packed_color<color_layout::ARGB> color24::pack_32bit (f32 alpha) const;
template <> packed_color<color_layout::ARGB> color24::pack_32bit (i32 alpha) const;


struct color32 {
    struct {
        union {
            struct { f32 r, g, b; };
            color24 rgb;
        };
        f32 a;
    };

    static color32 make ();
    static color32 make (f32 r, f32 g, f32 b, f32 a = 1.f);
    static color32 make (i32 r, i32 g, i32 b, i32 a = 255);
    static color32 make (u32 r, u32 g, u32 b, u32 a = 255);

    template <color_layout L>
    packed_color<L> pack () const = delete;

    template <color_layout L>
    static color32 unpack (packed_color<L>) = delete;

    template <color_layout L, typename T>
    static color32 unpack_24bit (packed_color<L>, T) = delete;
};

template <> packed_color<color_layout::ARGB> color32::pack () const;
template <> packed_color<color_layout::RGBA> color32::pack () const;

template <> color32 color32::unpack (packed_color<color_layout::ARGB>);
template <> color32 color32::unpack (packed_color<color_layout::RGBA>);

template <> color32 color32::unpack_24bit (packed_color<color_layout::RGB>, f32 alpha);
template <> color32 color32::unpack_24bit (packed_color<color_layout::RGB>, i32 alpha);


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


inline color24 lerp (const color24& a, const color24& b, f32 t) {
    return { lerp(a.r, b.r, t), lerp(a.g, b.g, t), lerp(a.b, b.b, t) };
}


inline color32 lerp (const color32& a, const color32& b, f32 t) {
    return { lerp(a.r, b.r, t), lerp(a.g, b.g, t), lerp(a.b, b.b, t), lerp(a.a, b.a, t) };
}


} // math
