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

    static mat3 zero();
    static mat3 eye();

    f32 det() const;
};

} // math
