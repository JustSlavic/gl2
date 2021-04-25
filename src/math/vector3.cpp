#include "vector3.hpp"
#include "vector2.hpp"


namespace math {

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
math::vector3 intersect_plane(
    math::vector3 r0, // ray origin
    math::vector3 r,  // ray direction
    math::vector3 p0, // any point on the plane
    math::vector3 n)  // normal of the plane
{
    r = r.normalized();
    n = n.normalized();

    f32 denom = math::dot(r, n);

    if (math::equal(denom, 0.f)) {
        return math::vector3::make(math::consts<f32>::nan());
    }

    f32 t = math::dot(p0 - r0, n)   / denom;
    return r0 + r * t;
}

} // math
