#include "vec2.hpp"
#include "vec3.hpp"

namespace math {

vec2::vec2 ()
    : x(0.f)
    , y(0.f) 
{}

vec2::vec2 (f32 v)
    : x(v)
    , y(v)
{}

vec2::vec2 (f32 x, f32 y)
    : x(x)
    , y(y)
{}

vec2::vec2 (const vec3& v)
    : x(v.x)
    , y(v.y)
{}


} // math
