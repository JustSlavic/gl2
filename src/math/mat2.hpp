#pragma once

namespace math {

struct mat2 {
    union {
        struct {
            f32 _11, _12;
            f32 _21, _22;
        };

        f32 at[4];
    };

    static mat4 zero();
    static mat4 eye();

    f32 det() const;
};

} // math
