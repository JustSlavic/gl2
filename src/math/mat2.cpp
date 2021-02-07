#include "mat2.hpp"

namespace math {

f32 mat2::det() const {
    return _11 * _22 - _12 * _21;
}

} // math
