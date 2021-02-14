#pragma once

#include <defines.h>


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

    static mat4 zero();
    static mat4 eye();

    inline const f32* data() const { return &at[0]; }
    inline       f32* data()       { return &at[0]; }

    f32 det() const;
};

f32  determinant(const mat4& matrix);

mat4 projection(f32 width, f32 height, f32 near, f32 far);
mat4 projection_fov(f32 fov, f32 width, f32 height, f32 near, f32 far);
mat4 projection_fov(f32 fov, f32 ratio, f32 near, f32 far);

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

} // math

