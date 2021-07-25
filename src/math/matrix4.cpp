#include "matrix4.hpp"

#include <cmath>

namespace math {

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


matrix4 look_at (vector3 to, vector3 from, vector3 up) {
    auto f = normalize(from - to);
    auto r = normalize(cross(f, up));
    auto u = cross(r, f);

    return {
         r.x,         r.y,         r.z,       0,
         u.x,         u.y,         u.z,       0,
        -f.x,        -f.y,        -f.z,       0,
        -dot(r, to), -dot(u, to), dot(f, to), 1,
    };
}


matrix4 translate (const matrix4& matrix, const vector3& displacement) {
    matrix4 transform {
        1, 0, 0, displacement.x,
        0, 1, 0, displacement.y,
        0, 0, 1, displacement.z,
        0, 0, 0, 1,
    };

    return transform * matrix;
}


matrix4 scale (const matrix4& matrix, const vector3& factor) {
    matrix4 transform {
        factor.x, 0, 0, 0,
        0, factor.y, 0, 0,
        0, 0, factor.z, 0,
        0, 0, 0,        1,
    };

    return transform * matrix;
}


} // math
