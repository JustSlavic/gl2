#include "vec4.hpp"

#include "vec2.hpp"
#include "vec3.hpp"


namespace math {

vec4::vec4 ()
    : x(0.f)
    , y(0.f)
    , z(0.f)
    , w(0.f)
{}

vec4::vec4 (f32 v)
    : x(v)
    , y(v)
    , z(v)
    , w(v)
{}

vec4::vec4 (f32 x, f32 y, f32 z, f32 w)
    : x(x)
    , y(y)
    , z(z)
    , w(w)
{}

vec4::vec4 (const vec2& v)
    : x(v.x)
    , y(v.y)
    , z(0.f)
    , w(0.f)
{}

vec4::vec4 (const vec2& v, f32 z, f32 w)
    : x(v.x)
    , y(v.y)
    , z(z)
    , w(w)
{}

vec4::vec4 (const vec2& v, const vec2& w)
    : x(v.x)
    , y(v.y)
    , z(w.x)
    , w(v.y)
{}

vec4::vec4 (const vec3& v)
    : x(v.x)
    , y(v.y)
    , z(v.z)
    , w(0.f)
{}

vec4::vec4 (const vec3& v, f32 w)
    : x(v.x)
    , y(v.y)
    , z(v.z)
    , w(w)
{}

} // math
