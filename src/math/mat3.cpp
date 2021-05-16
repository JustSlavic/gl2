#include "mat3.hpp"

namespace math {


mat3 mat3::zero () {
    mat3 result;

    for (int i = 0; i < 9; i++) {
        result.at[i] = 0;
    }

    return result;
}


mat3 mat3::diagonal (f32 v11, f32 v22, f32 v33) {
    mat3 result = mat3::zero();

    result._11 = v11;
    result._22 = v22;
    result._33 = v33;

    return result;
}


f32 mat3::det () const {
    return _11 * (_22 * _33 - _23 * _32) 
         - _12 * (_21 * _33 - _23 * _31) 
         + _33 * (_21 * _32 - _22 * _31);
}


f32 determinant (const mat3& matrix) {
    return matrix.det();
}


} // math
