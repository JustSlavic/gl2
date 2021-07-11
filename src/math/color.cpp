#include "color.hpp"
#include <stdio.h>


namespace math {


color24 color24::black = color24{ 0, 0, 0 };
color24 color24::white = color24{ 1, 1, 1 };
color24 color24::red   = color24{ 1, 0, 0 };
color24 color24::green = color24{ 0, 1, 0 };
color24 color24::blue  = color24{ 0, 0, 1 };

color32 color32::white = color32{1, 1, 1, 1};


color24 color24::make (f32 v) {
    ASSERT(0.f <= v && v <= 1.f);

    color24 color;
    color.r = color.g = color.b = v;

    return color;
}


color24 color24::make (i32 v) {
    ASSERT(0 <= v && v <= 255);

    color24 color;
    color.r = color.g = color.b = v / 255.f;

    return color;
}


color24 color24::make (u32 v) {
    ASSERT(v <= 255);

    color24 color;
    color.r = color.g = color.b = v / 255.f;

    return color;
}


color24 color24::make (f32 r, f32 g, f32 b) {
    ASSERT(0.f <= r && r <= 1.f);
    ASSERT(0.f <= g && g <= 1.f);
    ASSERT(0.f <= b && b <= 1.f);

    color24 color;
    color.r = r;
    color.g = g;
    color.b = b;

    return color;
}


color24 color24::make (i32 r, i32 g, i32 b) {
    ASSERT(0 <= r && r <= 255);
    ASSERT(0 <= g && g <= 255);
    ASSERT(0 <= b && b <= 255);

    color24 color;
    color.r = r / 255.f;
    color.g = g / 255.f;
    color.b = b / 255.f;

    return color;
}


color24 color24::make (u32 r, u32 g, u32 b) {
    ASSERT(r <= 255);
    ASSERT(g <= 255);
    ASSERT(b <= 255);

    color24 color;
    color.r = r / 255.f;
    color.g = g / 255.f;
    color.b = b / 255.f;

    return color;
}


template <>
color24 color24::unpack (packed_color<color_layout::RGB> color_) {
    u32 value = color_.get_value();
    ASSERT((value & (~0xFFFFFF)) == 0); // Everything except those bits is 0

    color24 color;
    color.r = ((value & 0xFF0000) >> 16) / 255.f;
    color.g = ((value & 0x00FF00) >>  8) / 255.f;
    color.b = ((value & 0x0000FF)      ) / 255.f;

    return color;
}


template <>
packed_color<color_layout::RGB> color24::pack () const {
    return packed_color<color_layout::RGB>{
           static_cast<u32>(r * 255.f) << 16
         | static_cast<u32>(g * 255.f) << 8
         | static_cast<u32>(b * 255.f)
    };
}


template <>
packed_color<color_layout::ARGB> color24::pack_32bit (f32 a) const {
    ASSERT(0.f <= a && a <= 1.f);

    return packed_color<color_layout::ARGB>{
           static_cast<u32>(a * 255.f) << 24
         | static_cast<u32>(r * 255.f) << 16
         | static_cast<u32>(g * 255.f) << 8
         | static_cast<u32>(b * 255.f)
    };
}


template <>
packed_color<color_layout::ARGB> color24::pack_32bit (i32 a) const {
    ASSERT(a <= 255);

    return packed_color<color_layout::ARGB>{
           a << 24
         | static_cast<u32>(r * 255.f) << 16
         | static_cast<u32>(g * 255.f) <<  8
         | static_cast<u32>(b * 255.f)
    };
}


color32 color32::make () {
    color32 color;
    color.r = color.g = color.b = color.a = 0.f;

    return color;
}


color32 color32::make (f32 r, f32 g, f32 b, f32 a /* = 1.f */) {
    ASSERT(0.f <= r && r <= 1.f);
    ASSERT(0.f <= g && g <= 1.f);
    ASSERT(0.f <= b && b <= 1.f);
    ASSERT(0.f <= a && a <= 1.f);

    color32 color;
    color.r = r;
    color.g = g;
    color.b = b;
    color.a = a;

    return color;
}


color32 color32::make (i32 r, i32 g, i32 b, i32 a /* = 255 */) {
    ASSERT(0 <= r && r <= 255);
    ASSERT(0 <= g && g <= 255);
    ASSERT(0 <= b && b <= 255);
    ASSERT(0 <= a && a <= 255);

    color32 color;
    color.r = r / 255.f;
    color.g = g / 255.f;
    color.b = b / 255.f;
    color.a = a / 255.f;

    return color;
}


color32 color32::make (u32 r, u32 g, u32 b, u32 a /* = 255 */) {
    ASSERT(r <= 255);
    ASSERT(g <= 255);
    ASSERT(b <= 255);
    ASSERT(a <= 255);

    color32 color;
    color.r = r / 255.f;
    color.g = g / 255.f;
    color.b = b / 255.f;
    color.a = a / 255.f;

    return color;
}


template <> packed_color<color_layout::ARGB> color32::pack () const {
    return packed_color<color_layout::ARGB>{
        static_cast<u32>(a * 255.f) << 24
            | static_cast<u32>(r * 255.f) << 16
            | static_cast<u32>(g * 255.f) << 8
            | static_cast<u32>(b * 255.f)
    };
}

template <> packed_color<color_layout::RGBA> color32::pack () const {
    return packed_color<color_layout::RGBA>{
           static_cast<u32>(a * 255.f) << 24
         | static_cast<u32>(r * 255.f) << 16
         | static_cast<u32>(g * 255.f) << 8
         | static_cast<u32>(b * 255.f)
    };
}


template <> color32 color32::unpack(packed_color<color_layout::ARGB> color_) {
    u32 value = color_.get_value();

    color32 color;
    color.a = ((value & 0xFF000000) >> 24) / 255.f;
    color.r = ((value & 0x00FF0000) >> 16) / 255.f;
    color.g = ((value & 0x0000FF00) >> 8) / 255.f;
    color.b = ((value & 0x000000FF)) / 255.f;

    return color;
}


template <> color32 color32::unpack(packed_color<color_layout::RGBA> color_) {
    u32 value = color_.get_value();

    color32 color;
    color.r = ((value & 0xFF000000) >> 24) / 255.f;
    color.g = ((value & 0x00FF0000) >> 16) / 255.f;
    color.b = ((value & 0x0000FF00) >> 8) / 255.f;
    color.a = ((value & 0x000000FF)) / 255.f;

    return color;
}


template <> color32 color32::unpack_24bit(packed_color<color_layout::RGB> color_, f32 a) {
    ASSERT(0.f <= a && a <= 1.f);

    u32 value = color_.get_value();
    ASSERT((value & (~0xFFFFFF)) == 0); // Everything except those bits is 0

    color32 color;
    color.r = ((value & 0xFF0000) >> 16) / 255.f;
    color.g = ((value & 0x00FF00) >> 8) / 255.f;
    color.b = ((value & 0x0000FF)) / 255.f;
    color.a = a;

    return color;
}


template <> color32 color32::unpack_24bit(packed_color<color_layout::RGB> color_, i32 a) {
    ASSERT(0 <= a && a <= 255);

    u32 value = color_.get_value();
    ASSERT((value & (~0xFFFFFF)) == 0); // Everything except those bits is 0

    color32 color;
    color.r = ((value & 0xFF0000) >> 16) / 255.f;
    color.g = ((value & 0x00FF00) >> 8) / 255.f;
    color.b = ((value & 0x0000FF)) / 255.f;
    color.a = a / 255.f;

    return color;
}


} // math
