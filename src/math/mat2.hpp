#pragma once

#include "vector2.hpp"

namespace math {

struct mat2 {
    union {
        struct {
            f32 _11, _12;
            f32 _21, _22;
        };
        f32 at[4];
    };

    static mat2 zero ();
    static mat2 eye  ();

    f32 det () const;
};

using matrix2 = mat2;

f32 determinant (const mat2& matrix);

inline mat2 operator + (const mat2& a, const mat2& b) {
    mat2 result;

    result._11 = a._11 + b._11;
    result._12 = a._12 + b._12;
    result._21 = a._21 + b._21;
    result._22 = a._22 + b._22;

    return result;
}

inline mat2 operator - (const mat2& a, const mat2& b) {
    mat2 result;

    result._11 = a._11 - b._11;
    result._12 = a._12 - b._12;
    result._21 = a._21 - b._21;
    result._22 = a._22 - b._22;

    return result;
}

inline mat2 operator * (const mat2& a, const mat2& b) {
    mat2 result;

    result._11 = a._11 * b._11 + a._12 * b._21;
    result._12 = a._11 * b._21 + a._12 * b._22;
    result._21 = a._21 * b._11 + a._22 * b._21;
    result._22 = a._21 * b._21 + a._22 * b._22;

    return result;
}

inline vector2 operator * (const mat2& m, const vector2& v) {
    return vector2(m._11 * v._1 + m._12 * v._2,
                m._21 * v._1 + m._22 * v._2);
}

inline vector2 operator * (const vector2& v, const mat2& m) {
    return vector2(v._1 * m._11 + v._2 * m._21,
                v._1 * m._12 + v._2 * m._22);
}

inline mat2 operator * (const mat2& m, f32 a) {
    mat2 result;

    result._11 = a * m._11;
    result._12 = a * m._12;
    result._21 = a * m._21;
    result._22 = a * m._22;

    return result;
}

inline mat2 operator * (f32 a, const mat2& m) {
    return m * a;
}

} // math
