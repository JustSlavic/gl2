#pragma once

#include <defines.hpp>
#include "vector2.hpp"


namespace math {

struct matrix2 {
    union {
        struct {
            f32 _11, _12;
            f32 _21, _22;
        };
        f32 at[4];
    };

    static inline matrix2 zero () {
        return matrix2{
            0, 0,
            0, 0
        };
    }

    static inline matrix2 identity () {
        return matrix2{
            1, 0,
            0, 1
        };
    }

    inline f32 det () const { return _11 * _22 - _12 * _21; }
};

inline f32 determinant (const matrix2& matrix) { return matrix.det(); }

inline matrix2 operator + (const matrix2& a, const matrix2& b) {
    return matrix2{
        a._11 + b._11, a._12 + b._12,
        a._21 + b._21, a._22 + b._22
    };
}

inline matrix2 operator - (const matrix2& a, const matrix2& b) {
    return matrix2{
        a._11 - b._11, a._12 - b._12,
        a._21 - b._21, a._22 - b._22
    };
}

inline matrix2 operator * (const matrix2& a, const matrix2& b) {
    return matrix2{
        a._11 * b._11 + a._12 * b._21, a._11 * b._12 + a._12 * b._22,
        a._21 * b._11 + a._22 * b._21, a._21 * b._12 + a._22 * b._22,
    };
}

inline vector2 operator * (const matrix2& m, const vector2& v) {
    return vector2{
        m._11 * v._1 + m._12 * v._2,
        m._21 * v._1 + m._22 * v._2
    };
}

inline vector2 operator * (const vector2& v, const matrix2& m) {
    return vector2{
        v._1 * m._11 + v._2 * m._21,
        v._1 * m._12 + v._2 * m._22
    };
}

inline matrix2 operator * (const matrix2& m, f32 a) {
    return matrix2{
        a * m._11, a * m._12,
        a * m._21, a * m._22
    };
}

inline matrix2 operator * (f32 a, const matrix2& m) {
    return m * a;
}

inline bool operator == (const matrix2& lhs, const matrix2& rhs) {
    for (int i = 0; i < 4; i++) {
        if (lhs.at[i] != rhs.at[i]) return false;
    }

    return true;
}

inline bool operator != (const matrix2& lhs, const matrix2& rhs) {
    return !(lhs == rhs);
}

} // math

#ifdef UNITY_BUILD
#include "matrix2.cpp"
#endif
