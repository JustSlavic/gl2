#pragma once

#include <glm/glm.hpp>
#include <math.hpp>

struct Camera2D {
    math::vector3 position; // world coordinates

    Camera2D();

    // p is in world coordinates
    void move_to(math::vector3 p) { position = p; }

    glm::mat4 get_view_matrix() const;
    math::mat4 get_view_matrix_math () const;

    math::vector3 get_forward_vector () const;
    math::vector3 get_up_vector      () const;
};
