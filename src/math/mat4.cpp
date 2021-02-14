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


// 11 12 13 14
// 21 22 23 24
// 31 32 33 34
// 41 42 43 44
f32 mat4::det() const {
    return _11 * (_22 * (_33 * _44 - _34 * _43) - _23 * (_32 * _44 - _34 * _42) + _24 * (_32 * _43 - _33 * _42))
         - _12 * (_21 * (_33 * _44 - _34 * _43) - _23 * (_21 * _44 - _24 * _41) + _24 * (_31 * _43 - _33 * _41))
         + _13 * (_21 * (_32 * _44 - _34 * _42) - _22 * (_31 * _44 - _34 * _41) + _24 * (_31 * _42 - _32 * _41))
         - _14 * (_21 * (_32 * _43 - _33 * _42) - _22 * (_31 * _43 - _33 * _41) + _23 * (_31 * _42 - _32 * _41));
}


f32 determinant(const mat4& matrix) {
    return matrix.det();
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
