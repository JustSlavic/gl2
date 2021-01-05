#include <defines.h>
#include <cmath>
#include <cfloat>

namespace math {

template <typename T>
struct consts {};

template <>
struct consts<f32> {
    using self = f32;

    static constexpr self eps() noexcept { return FLT_EPSILON; }
    static constexpr self min() noexcept { return FLT_MIN; }
    static constexpr self max() noexcept { return FLT_MAX; }
    static constexpr self inf() noexcept { return HUGE_VALF; }
    static constexpr self nan() noexcept { return NAN; }

    static constexpr self e       () noexcept { return 2.7182818284590452354; }  // e
    static constexpr self pi      () noexcept { return 3.14159265358979323846; } // pi
    static constexpr self pi_2    () noexcept { return 1.57079632679489661923; } // pi/2
    static constexpr self pi1     () noexcept { return 0.31830988618379067154; } // 1/pi
    static constexpr self sqrt_2  () noexcept { return 1.41421356237309504880; } // sqrt(2)
    static constexpr self sqrt1_2 () noexcept { return 0.70710678118654752440; } // 1/sqrt(2)
};

template <>
struct consts<f64> {
    using self = f64;

    static constexpr self eps() noexcept { return DBL_EPSILON; }
    static constexpr self min() noexcept { return DBL_MIN; }
    static constexpr self max() noexcept { return DBL_MAX; }
    static constexpr self inf() noexcept { return HUGE_VAL; }
};

} // math
