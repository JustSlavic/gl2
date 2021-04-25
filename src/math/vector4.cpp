#include "vector4.hpp"

#include "vector2.hpp"
#include "vector3.hpp"


namespace math {

vector4::vector4 ()
    : x(0.f)
    , y(0.f)
    , z(0.f)
    , w(0.f)
{}

vector4::vector4 (f32 v)
    : x(v)
    , y(v)
    , z(v)
    , w(v)
{}

vector4::vector4 (f32 x, f32 y, f32 z, f32 w)
    : x(x)
    , y(y)
    , z(z)
    , w(w)
{}

vector4::vector4 (const vector2& v)
    : x(v.x)
    , y(v.y)
    , z(0.f)
    , w(0.f)
{}

vector4::vector4 (const vector2& v, f32 z, f32 w)
    : x(v.x)
    , y(v.y)
    , z(z)
    , w(w)
{}

vector4::vector4 (const vector2& v, const vector2& w)
    : x(v.x)
    , y(v.y)
    , z(w.x)
    , w(v.y)
{}

vector4::vector4 (const vector3& v)
    : x(v.x)
    , y(v.y)
    , z(v.z)
    , w(0.f)
{}

vector4::vector4 (const vector3& v, f32 w)
    : x(v.x)
    , y(v.y)
    , z(v.z)
    , w(w)
{}

} // math
