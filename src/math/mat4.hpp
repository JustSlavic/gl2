#pragma once

#include <defines.h>
#include "vector4.hpp"


namespace math {


struct mat4 {
    union {
        struct {
            f32 _11, _12, _13, _14;
            f32 _21, _22, _23, _24;
            f32 _31, _32, _33, _34;
            f32 _41, _42, _43, _44;
        };

        f32 at[16];
    };

    static mat4 zero ();
    static mat4 identity ();

    inline const f32* data () const { return &at[0]; }
    inline       f32* data ()       { return &at[0]; }

    f32 det () const;
};


using matrix4 = mat4;


f32 determinant (const mat4& matrix);

matrix4 projection (f32 width, f32 height, f32 near, f32 far);
matrix4 projection_fov (f32 fov, f32 width, f32 height, f32 near, f32 far);
matrix4 projection_fov (f32 fov, f32 ratio, f32 near, f32 far);
matrix4 projection_far (f32 width, f32 height, f32 near, f32 far);


inline mat4 operator + (const mat4& lhs, const mat4& rhs) {
    mat4 result;

    for (size_t i = 0; i < 16; i++) {
        result.at[i] = lhs.at[i] + rhs.at[i];
    }

    return result;
}


inline mat4 operator * (const mat4& lhs, const mat4& rhs) {
    mat4 result;

    for (size_t i = 0; i < 4; i++) {
        for (size_t j = 0; j < 4; j++) {
            for (size_t k = 0; k < 4; k++) {
                result.at[i*4 + j] += lhs.at[i*4 + k] * rhs.at[k*4 + j];
            }
        }
    }

    return result;
}


inline vector4 operator * (const mat4& m, const vector4& v) {
    vector4 result;

    result._1 = m._11 * v._1 + m._12 * v._2 + m._13 * v._3 + m._14 * v._4;
    result._2 = m._21 * v._1 + m._22 * v._2 + m._23 * v._3 + m._24 * v._4;
    result._3 = m._31 * v._1 + m._32 * v._2 + m._33 * v._3 + m._34 * v._4;
    result._4 = m._41 * v._1 + m._42 * v._2 + m._43 * v._3 + m._44 * v._4;

    return result;
}


inline vector4 operator * (const vector4& v, const matrix4& m) {
    vector4 result;

    result._1 = v._1 * m._11 + v._2 * m._21 + v._3 * m._31 + v._4 * m._41;
    result._2 = v._1 * m._12 + v._2 * m._22 + v._3 * m._32 + v._4 * m._42;
    result._3 = v._1 * m._13 + v._2 * m._23 + v._3 * m._33 + v._4 * m._43;
    result._4 = v._1 * m._14 + v._2 * m._24 + v._3 * m._34 + v._4 * m._44;

    return result;
}


} // math

