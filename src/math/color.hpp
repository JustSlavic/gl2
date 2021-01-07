#pragma once

namespace math {

struct color_rgba;
struct color_rgb {
    struct { f32 r; f32 g; f32 b; };

             color_rgb ();
    explicit color_rgb (i32 color_24bit);

    // i32  to_24bit () const;  // @usability under question
    i32  to_32bit (i32 alpha) const;
    vec3 to_vec3  () const;
}

struct color_rgba {
    struct {
        union {
            struct { f32 r; f32 g; f32 b };
            color_rgb rgb;
        };
        f32 a;
    };

             color_rgba () = default;
    explicit color_rgba (i32 color_32bit);

    // i32  to_24bit () const;  // @usability under question
    i32  to_32bit () const;
    vec3 to_vec3  () const;
    vec4 to_vec4  () const;
}

} // math
