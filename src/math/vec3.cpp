#include "vec3.hpp"
#include "vec2.hpp"

namespace math {

vec3::vec3 ()                     
    : x(0.f)
    , y(0.f)
    , z(0.f) 
{}

vec3::vec3 (f32 v)                
    : x(v)
    , y(v)
    , z(v)   
{}

vec3::vec3 (f32 x, f32 y, f32 z)  
    : x(x)
    , y(y)
    , z(z)   
{}

vec3::vec3 (const vec2& v)        
    : x(v.x)
    , y(v.y)
    , z(0.f) 
{}

vec3::vec3 (const vec2& v, f32 z) 
    : x(v.x)
    , y(v.y)
    , z(z)   
{}

vec3::vec3 (f32 x, const vec2& v) 
    : x(x)
    , y(v.x)
    , z(v.y) 
{}

} // math
