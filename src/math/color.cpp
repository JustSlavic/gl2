#include "color.hpp"


namespace math {


color_rgb color_rgb::make(f32 r, f32 g, f32 b) {
    ASSERT(0.f <= r && r <= 1.f);
    ASSERT(0.f <= g && g <= 1.f);
    ASSERT(0.f <= b && b <= 1.f);

    color_rgb color;
    color.r = r;
    color.g = g;
    color.b = b;

    return color;
}

color_rgb color_rgb::from_24bit (u32 color_24bit) {
    ASSERT((color_24bit & (~0xFFFFFF)) == 0); // everything except those bits is 0

    color_rgb color;
    color.r = ((color_24bit & 0xFF0000) >> 16) / 255.f;
    color.g = ((color_24bit & 0x00FF00) >> 8) / 255.f;
    color.b = ((color_24bit & 0x0000FF)) / 255.f;

    return color;
}

color_rgb color_rgb::from_32bit (u32 color_32bit) {
    color_rgb color;
    color.r = ((color_32bit & 0xFF000000) >> 24) / 255.f;
    color.g = ((color_32bit & 0x00FF0000) >> 16) / 255.f;
    color.b = ((color_32bit & 0x0000FF00) >> 8) / 255.f;

    return color;
}

u32 color_rgb::to_24bit () const {
    return static_cast<u32>(r * 255.f) << 16 |
           static_cast<u32>(g * 255.f) << 8  |
           static_cast<u32>(b * 255.f);
}

u32 color_rgb::to_32bit (f32 a) const {
    ASSERT(0.f <= a && a <= 1.f);

    return static_cast<u32>(r * 255.f) << 24 |
           static_cast<u32>(g * 255.f) << 16 |
           static_cast<u32>(b * 255.f) << 8  |
           static_cast<u32>(a * 255.f);
}

u32 color_rgb::to_32bit (i32 a) const {
  return to_32bit(static_cast<u32>(a));
}

u32 color_rgb::to_32bit (u32 a) const {
    ASSERT((a & (~0xFF)) == 0); // everything except last two bits is 0

    return static_cast<u32>(r * 255.f) << 24 |
           static_cast<u32>(g * 255.f) << 16 |
           static_cast<u32>(b * 255.f) << 8  |
           a;
}

color_rgba color_rgba::make(f32 r, f32 g, f32 b, f32 a) {
    ASSERT(0.f <= r && r <= 1.f);
    ASSERT(0.f <= g && g <= 1.f);
    ASSERT(0.f <= b && b <= 1.f);
    ASSERT(0.f <= a && a <= 1.f);

    color_rgba color;
    color.r = r;
    color.g = g;
    color.b = b;
    color.a = a;

    return color;
}

color_rgba color_rgba::from_24bit (u32 color_24bit, f32 alpha) {
    ASSERT((color_24bit & (~0xFFFFFF)) == 0);
    ASSERT(0.f <= alpha && alpha <= 1.f); 

    color_rgba color;
    color.r = ((color_24bit & 0xFF0000) >> 16) / 255.f;
    color.g = ((color_24bit & 0x00FF00) >> 8) / 255.f;
    color.b = ((color_24bit & 0x0000FF)) / 255.f;
    color.a = alpha;

    return color;
}

color_rgba color_rgba::from_24bit (u32 color_24bit, i32 alpha) {
  return from_24bit(color_24bit, static_cast<u32>(alpha));
}

color_rgba color_rgba::from_24bit (u32 color_24bit, u32 alpha) {
    ASSERT((color_24bit & (~0xFFFFFF)) == 0); 
    ASSERT((alpha & (~0xFF)) == 0); 

    color_rgba color;
    color.r = ((color_24bit & 0xFF0000) >> 16) / 255.f;
    color.g = ((color_24bit & 0x00FF00) >> 8) / 255.f;
    color.b = ((color_24bit & 0x0000FF)) / 255.f;
    color.a = alpha / 255.f;

    return color;
}

color_rgba color_rgba::from_32bit (u32 color_32bit) {
    color_rgba color;
    color.r = ((color_32bit & 0xFF000000) >> 24) / 255.f;
    color.g = ((color_32bit & 0x00FF0000) >> 16) / 255.f;
    color.b = ((color_32bit & 0x0000FF00) >> 8) / 255.f;
    color.a = ((color_32bit & 0x000000FF)) / 255.f;

    return color;
}

u32 color_rgba::to_24bit () const {
    return static_cast<u32>(r * 255.f) << 16 |
           static_cast<u32>(g * 255.f) << 8  |
           static_cast<u32>(b * 255.f);
}

u32 color_rgba::to_32bit () const {
    return static_cast<u32>(r * 255.f) << 24 |
           static_cast<u32>(g * 255.f) << 16 |
           static_cast<u32>(b * 255.f) << 8  |
           static_cast<u32>(a * 255.f);
}


} // math
