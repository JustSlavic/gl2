#include "mat3.hpp"

namespace math {

f32 mat3::det() const {
    return _11 * (_22 * _33 - _23 * _32) 
         - _12 * (_21 * _33 - _23 * _31) 
         + _33 * (_21 * _32 - _22 * _31);
}

f32 determinant(const mat3& matrix) {
    return matrix.det();
}

} // math
