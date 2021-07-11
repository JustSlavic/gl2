#pragma once

#include <defines.hpp>
#include "vector4.hpp"


namespace math {


struct matrix4 {
    union {
        struct {
            f32 _11, _12, _13, _14;
            f32 _21, _22, _23, _24;
            f32 _31, _32, _33, _34;
            f32 _41, _42, _43, _44;
        };

        f32 at[16];
    };

    static matrix4 zero () {
        return matrix4{
            0, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0
        };
    }

    static matrix4 identity () {
        return matrix4{
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        };
    }

    inline const f32* data () const { return &at[0]; }
    inline       f32* data ()       { return &at[0]; }

    f32 det () const;
};


using mat4 = matrix4;


inline f32 determinant(const matrix4& matrix) { return matrix.det(); }

matrix4 projection (f32 width, f32 height, f32 near, f32 far);
matrix4 projection_fov (f32 fov, f32 width, f32 height, f32 near, f32 far);
matrix4 projection_fov (f32 fov, f32 ratio, f32 near, f32 far);
matrix4 projection_1 (f32 width, f32 height, f32 near, f32 far);


inline matrix4 operator + (const matrix4& lhs, const matrix4& rhs) {
    matrix4 result;

    for (size_t i = 0; i < 16; i++) {
        result.at[i] = lhs.at[i] + rhs.at[i];
    }

    return result;
}


inline matrix4 operator * (const matrix4& lhs, const matrix4& rhs) {
    matrix4 result = matrix4::zero();

    for (size_t i = 0; i < 4; i++) {
        for (size_t j = 0; j < 4; j++) {
            for (size_t k = 0; k < 4; k++) {
                result.at[i*4 + j] += lhs.at[i*4 + k] * rhs.at[k*4 + j];
            }
        }
    }

    return result;
}


inline vector4 operator * (const matrix4& m, const vector4& v) {
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


inline bool operator == (const matrix4& lhs, const matrix4& rhs) {
    for (int i = 0; i < 16; i++) {
        if (lhs.at[i] != rhs.at[i]) return false;
    }

    return true;
}


inline bool operator != (const matrix4& lhs, const matrix4& rhs) {
    return !(lhs == rhs);
}


inline void print_matrix4(matrix4 m) {
    printf("|%5.2f, %5.2f, %5.2f, %5.2f|\n"
           "|%5.2f, %5.2f, %5.2f, %5.2f|\n"
           "|%5.2f, %5.2f, %5.2f, %5.2f|\n"
           "|%5.2f, %5.2f, %5.2f, %5.2f|\n",
        m._11, m._12, m._13, m._14,
        m._21, m._22, m._23, m._24,
        m._31, m._32, m._33, m._34,
        m._41, m._42, m._43, m._44
    );
}


} // math

