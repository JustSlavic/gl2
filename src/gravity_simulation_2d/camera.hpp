#pragma once

#include <math.hpp>

struct Camera2D {
    math::vector3 position; // world coordinates

    Camera2D();

    // p is in world coordinates
    void move_to(math::vector3 p) { position = p; }

    math::matrix4 get_view_matrix() const;

    math::vector3 get_forward_vector () const;
    math::vector3 get_up_vector      () const;
};

#ifdef UNITY_BUILD
#include "camera.cpp"
#endif
