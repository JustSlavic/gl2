#include "mat4.hpp"

#include <cmath>

namespace math {

mat4 mat4::zero() {
    mat4 result;

    for (size_t i = 0; i < 16; i++) {
        result.at[i] = 0;
    }

    return result;
}

mat4 mat4::eye() {
    mat4 result;

    for (size_t i = 0; i < 16; i++) {
        result.at[i] = 0;
    }

    result._11 = 1.f;
    result._22 = 1.f;
    result._33 = 1.f;
    result._44 = 1.f;

    return result;
}

// w - width
// h - height
// n - near clip distance
// f - far clip distance
mat4 projection(f32 w, f32 h, f32 n, f32 f) {
    auto a = mat4::zero();

    a._11 = 2 * n / w;
    a._22 = 2 * n / h;
    a._33 = -(f + n) / (f - n);
    a._34 = -1.f;
    a._43 = -2 * f * n / (f - n);

    return a;
}

// fov - field of view (angle in radians)
// w - width
// h - height
// n - near clip distance
// f - far clip distance
mat4 projection_fov(f32 fov, f32 w, f32 h, f32 n, f32 f) {
    auto a = mat4::zero();

    // tg(fov / 2) == w / n
    // n / w == ctg(fov / 2)
    // n / w == 1 / tg(fov / 2)
    f32 tf2 = 1.f / ::std::tan(.5f * fov);

    a._11 = tf2 * h / w;
    a._22 = tf2;
    a._33 = -(f + n) / (f - n);
    a._34 = -1.f;
    a._43 = -2 * f * n / (f - n);

    return a;
}

// fov - field of view (angle in radians)
// ratio - aspect ratio of the viewport
// n - near clip distance
// f - far clip distance
mat4 projection_fov(f32 fov, f32 ratio, f32 n, f32 f) {
    auto a = mat4::zero();

    f32 tf2 = 1.f / ::std::tan(.5f * fov);

    a._11 = tf2 / ratio;
    a._22 = tf2;
    a._33 = -(f + n) / (f - n);
    a._34 = -1.f;
    a._43 = -2 * f * n / (f - n);

    return a;
}

} // math
