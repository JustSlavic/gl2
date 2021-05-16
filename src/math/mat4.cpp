#include "mat4.hpp"

#include <cmath>

namespace math {

matrix4 mat4::zero() {
    matrix4 result;

    for (size_t i = 0; i < 16; i++) {
        result.at[i] = 0;
    }

    return result;
}

matrix4 mat4::identity() {
    matrix4 result;

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
float32 matrix4::det() const {
    return _11 * (_22 * (_33 * _44 - _34 * _43) - _23 * (_32 * _44 - _34 * _42) + _24 * (_32 * _43 - _33 * _42))
         - _12 * (_21 * (_33 * _44 - _34 * _43) - _23 * (_21 * _44 - _24 * _41) + _24 * (_31 * _43 - _33 * _41))
         + _13 * (_21 * (_32 * _44 - _34 * _42) - _22 * (_31 * _44 - _34 * _41) + _24 * (_31 * _42 - _32 * _41))
         - _14 * (_21 * (_32 * _43 - _33 * _42) - _22 * (_31 * _43 - _33 * _41) + _23 * (_31 * _42 - _32 * _41));
}


float32 determinant(const matrix4& matrix) {
    return matrix.det();
}

mat4 projection(f32 width, f32 height, f32 near, f32 far) {
    auto a = mat4::zero();

    a._11 = 2 * near / width;
    a._22 = 2 * near / height;
    a._33 = -(far + near) / (far - near);
    a._34 = -1.f;
    a._43 = -2 * far * near / (far - near);

    return a;
}

mat4 projection_fov(f32 fov, f32 width, f32 height, f32 near, f32 far) {
    auto a = mat4::zero();

    // tg(fov / 2) == width / near
    // near / width == ctg(fov / 2)
    // near / width == 1 / tg(fov / 2)
    f32 tf2 = 1.f / ::std::tan(.5f * fov);

    a._11 = tf2 * height / width;
    a._22 = tf2;
    a._33 = -(far + near) / (far - near);
    a._34 = -1.f;
    a._43 = -2 * far * near / (far - near);

    return a;
}

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

matrix4 projection_1(f32 width, f32 height, f32 near, f32 far) {
    auto a = matrix4::zero();

    a._11 = 2.f * far / width;
    a._22 = 2.f * far / height;
    a._33 = -(far + near) / (far - near);
    a._34 = -1.f;
    a._43 = -2.f * far * near / (far - near);

    return a;
}


} // math
