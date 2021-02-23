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

//
// Let's say ray intersects plane in point p, thus vector(p0,p) = p - p0 lies on the plane.
//   => dot(p - p0, n) == 0
// It takes t units along the ray, to get to the point p from the r0 (ray origin).
//   => p = r0 + r*t
// Substitute p into dot product and you get
//   => dot(r0 + r*t - p0, n) == 0
//   => dot(r0 - p0, n) + t*dot(r, n) == 0
//   => t = dot(p0 - r0, n) / dot(r, n)
// Substitute into equation 2 and you're done.
//
math::vec3 intersect_plane(
    math::vec3 r0, // ray origin
    math::vec3 r,  // ray direction
    math::vec3 p0, // any point on the plane
    math::vec3 n)  // normal of the plane
{
    r = r.normalized();
    n = n.normalized();

    f32 denom = math::dot(r, n);

    if (math::equal(denom, 0.f)) {
        return math::vec3(math::consts<f32>::nan());
    }

    f32 t = math::dot(p0 - r0, n)   / denom;
    return r0 + r * t;
}

} // math
