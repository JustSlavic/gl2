#pragma once

#include <glm/glm.hpp>
#include <math.hpp>

struct Camera2D {
    math::vector3 position;

    Camera2D();

    glm::mat4 get_view_matrix() const;

    math::vector3 get_forward_vector () const;
    math::vector3 get_up_vector      () const;
};
