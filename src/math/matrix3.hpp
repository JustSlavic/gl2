#pragma once

#include <defines.hpp>
#include "vector3.hpp"


namespace math {


struct matrix3 {
    union {
        struct {
            f32 _11, _12, _13;
            f32 _21, _22, _23;
            f32 _31, _32, _33;
        };

        f32 at[9];
    };

    static inline matrix3 zero () {
        return matrix3{
            0, 0, 0,
            0, 0, 0,
            0, 0, 0
        };
    }

    static inline matrix3 identity () {
        return matrix3{
            1, 0, 0,
            0, 1, 0,
            0, 0, 1
        };
    }

    inline f32 det () const {
        return _11 * (_22 * _33 - _23 * _32)
             - _12 * (_21 * _33 - _23 * _31)
             + _13 * (_21 * _32 - _22 * _31);
    }
};


using mat3 = matrix3;


inline f32 determinant (const matrix3& matrix) { return matrix.det(); }


inline matrix3 operator + (const matrix3& a, const matrix3& b) {
    matrix3 result;

    for (int i = 0; i < 9; i++) {
        result.at[i] = a.at[i] + b.at[i];
    }

    return result;
}


inline matrix3 operator - (const matrix3& a, const matrix3& b) {
    matrix3 result;

    for (int i = 0; i < 9; i++) {
        result.at[i] = a.at[i] - b.at[i];
    }

    return result;
}


inline matrix3 operator * (const matrix3& a, const matrix3& b) {
    matrix3 result;

    result._11 = a._11 * b._11 + a._12 * b._21 + a._13 * b._31;
    result._12 = a._11 * b._12 + a._12 * b._22 + a._13 * b._32;
    result._13 = a._11 * b._13 + a._12 * b._23 + a._13 * b._33;

    result._21 = a._21 * b._11 + a._22 * b._21 + a._23 * b._31;
    result._22 = a._21 * b._12 + a._22 * b._22 + a._23 * b._32;
    result._23 = a._21 * b._13 + a._22 * b._23 + a._23 * b._33;

    result._31 = a._31 * b._11 + a._32 * b._21 + a._33 * b._31;
    result._32 = a._31 * b._12 + a._32 * b._22 + a._33 * b._32;
    result._33 = a._31 * b._13 + a._32 * b._23 + a._33 * b._33;

    return result;
}


inline vector3 operator * (const matrix3& m, const vector3& v) {
    return { m._11 * v._1 + m._12 * v._2 + m._13 * v._3,
             m._21 * v._1 + m._22 * v._2 + m._23 * v._3,
             m._31 * v._1 + m._32 * v._2 + m._33 * v._3 };
}


inline vector3 operator * (const vector3& v, const matrix3& m) {
    return { v._1 * m._11 + v._2 * m._21 + v._3 * m._31,
             v._1 * m._12 + v._2 * m._22 + v._3 * m._32,
             v._1 * m._13 + v._2 * m._23 + v._3 * m._33 };
}


inline matrix3 operator * (const matrix3& m, f32 a) {
    matrix3 result;

    for (int i = 0; i < 9; i++) {
        result.at[i] = m.at[i] * a;
    }

    return result;
}


inline matrix3 operator * (f32 a, const matrix3& m) {
    return m * a;
}

inline bool operator == (const matrix3& lhs, const matrix3& rhs) {
    for (int i = 0; i < 9; i++) {
        if (lhs.at[i] != rhs.at[i]) return false;
    }

    return true;
}

inline bool operator != (const matrix3& lhs, const matrix3& rhs) {
    return !(lhs == rhs);
}


} // math
