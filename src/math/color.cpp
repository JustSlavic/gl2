#include "color.hpp"


namespace math {


color24 color24::make(f32 v) {
  ASSERT(0.f <= v && v <= 1.f);

  color24 color;
  color.r = color.g = color.b = v;
  
  return color;
}

color24 color24::make(f32 r, f32 g, f32 b) {
    ASSERT(0.f <= r && r <= 1.f);
    ASSERT(0.f <= g && g <= 1.f);
    ASSERT(0.f <= b && b <= 1.f);

    color24 color;
    color.r = r;
    color.g = g;
    color.b = b;

    return color;
}

color24 color24::from_24bit (u32 color_24bit) {
    ASSERT((color_24bit & (~0xFFFFFF)) == 0); // everything except those bits is 0

    color24 color;
    color.r = ((color_24bit & 0xFF0000) >> 16) / 255.f;
    color.g = ((color_24bit & 0x00FF00) >> 8) / 255.f;
    color.b = ((color_24bit & 0x0000FF)) / 255.f;

    return color;
}

color24 color24::from_32bit (u32 color_32bit) {
    color24 color;
    color.r = ((color_32bit & 0xFF000000) >> 24) / 255.f;
    color.g = ((color_32bit & 0x00FF0000) >> 16) / 255.f;
    color.b = ((color_32bit & 0x0000FF00) >> 8) / 255.f;

    return color;
}

u32 color24::to_24bit () const {
    return static_cast<u32>(r * 255.f) << 16 |
           static_cast<u32>(g * 255.f) << 8  |
           static_cast<u32>(b * 255.f);
}

u32 color24::to_32bit (f32 a) const {
    ASSERT(0.f <= a && a <= 1.f);

    return static_cast<u32>(r * 255.f) << 24 |
           static_cast<u32>(g * 255.f) << 16 |
           static_cast<u32>(b * 255.f) << 8  |
           static_cast<u32>(a * 255.f);
}

u32 color24::to_32bit (i32 a) const {
  return to_32bit(static_cast<u32>(a));
}

u32 color24::to_32bit (u32 a) const {
    ASSERT((a & (~0xFF)) == 0); // everything except last two bits is 0

    return static_cast<u32>(r * 255.f) << 24 |
           static_cast<u32>(g * 255.f) << 16 |
           static_cast<u32>(b * 255.f) << 8  |
           a;
}

color32 color32::make(f32 r, f32 g, f32 b, f32 a) {
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

color32 color32::from_24bit (u32 color_24bit, f32 alpha) {
    ASSERT((color_24bit & (~0xFFFFFF)) == 0);
    ASSERT(0.f <= alpha && alpha <= 1.f); 

    color32 color;
    color.r = ((color_24bit & 0xFF0000) >> 16) / 255.f;
    color.g = ((color_24bit & 0x00FF00) >> 8) / 255.f;
    color.b = ((color_24bit & 0x0000FF)) / 255.f;
    color.a = alpha;

    return color;
}

color32 color32::from_24bit (u32 color_24bit, i32 alpha) {
  return from_24bit(color_24bit, static_cast<u32>(alpha));
}

color32 color32::from_24bit (u32 color_24bit, u32 alpha) {
    ASSERT((color_24bit & (~0xFFFFFF)) == 0); 
    ASSERT((alpha & (~0xFF)) == 0); 

    color32 color;
    color.r = ((color_24bit & 0xFF0000) >> 16) / 255.f;
    color.g = ((color_24bit & 0x00FF00) >> 8) / 255.f;
    color.b = ((color_24bit & 0x0000FF)) / 255.f;
    color.a = alpha / 255.f;

    return color;
}

color32 color32::from_32bit (u32 color_32bit) {
    color32 color;
    color.r = ((color_32bit & 0xFF000000) >> 24) / 255.f;
    color.g = ((color_32bit & 0x00FF0000) >> 16) / 255.f;
    color.b = ((color_32bit & 0x0000FF00) >> 8) / 255.f;
    color.a = ((color_32bit & 0x000000FF)) / 255.f;

    return color;
}

u32 color32::to_24bit () const {
    return static_cast<u32>(r * 255.f) << 16 |
           static_cast<u32>(g * 255.f) << 8  |
           static_cast<u32>(b * 255.f);
}

u32 color32::to_32bit () const {
    return static_cast<u32>(r * 255.f) << 24 |
           static_cast<u32>(g * 255.f) << 16 |
           static_cast<u32>(b * 255.f) << 8  |
           static_cast<u32>(a * 255.f);
}


} // math
