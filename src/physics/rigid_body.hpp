#ifndef GL2_SRC_PHYSICS_RIDID_BODY_HPP
#define GL2_SRC_PHYSICS_RIDID_BODY_HPP

#include <math.hpp>


namespace physics {


struct static_segment_2d {
    math::vector2 start;
    math::vector2 end;

    inline math::vector2 get_normal() {
        auto v = start - end;
        return math::normalize(math::vector2{ v.y, v.x });
    }
};


struct dynamic_body_2d {
    math::vector2 position; // aka origin of the local coordinate system in world's coordinate system
    math::vector2 velocity; // in world coordinates
    float32 mass;

    dynamic_body_2d(math::vector2 p, math::vector2 v, float32 m)
        : position(p)
        , velocity(v)
        , mass(m)
    {}

    // Distance from this body to the point in world coordinates
    float32 get_distance_to(math::vector2 p) {
        return (position - p).length();
    }
};


} // physics


#endif // GL2_SRC_PHYSICS_RIDID_BODY_HPP
