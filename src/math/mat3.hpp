#pragma once

namespace math {

struct mat3 {
    union {
        struct {
            f32 _11, _12, _13;
            f32 _21, _22, _23;
            f32 _31, _32, _33;
        };

        f32 at[9];
    };

    static mat3 zero ();
    static mat3 eye  ();

    f32 det () const;
};

using matrix3 = mat3;

f32 determinant (const mat3& matrix);

inline mat3 operator + (const mat3& a, const mat3& b) {
    mat3 result;

    for (int i = 0; i < 9; i++) {
        result.at[i] = a.at[i] + b.at[i];
    }

    return result;
}

inline mat3 operator - (const mat3& a, const mat3& b) {
    mat3 result;

    for (int i = 0; i < 9; i++) {
        result.at[i] = a.at[i] - b.at[i];
    }

    return result;
}

inline mat3 operator * (const mat3& a, const mat3& b) {
    mat3 result;

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

inline vector3 operator * (const mat3& m, const vector3& v) {
    return vector3(m._11 * v._1 + m._12 * v._2 + m._13 * v._3,
                m._21 * v._1 + m._22 * v._2 + m._23 * v._3,
                m._31 * v._1 + m._32 * v._2 + m._33 * v._3);
}

inline vector3 operator * (const vector3& v, const mat3& m) {
    return vector3(v._1 * m._11 + v._2 * m._21 + v._3 * m._31,
                v._1 * m._12 + v._2 * m._22 + v._3 * m._32,
                v._1 * m._13 + v._2 * m._23 + v._3 * m._33);
}

inline mat3 operator * (const mat3& m, f32 a) {
    mat3 result;

    for (int i = 0; i < 9; i++) {
        result.at[i] = m.at[i] * a;
    }

    return result;
}

inline mat3 operator * (f32 a, const mat3& m) {
    return m * a;
}

} // math
