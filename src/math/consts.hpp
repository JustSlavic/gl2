#include <defines.h>
#include <cmath>

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
