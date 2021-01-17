#pragma once

#include <glm/glm.hpp>
#include <math.hpp>

struct Camera2D {
    glm::vec3 position;

    Camera2D();

    glm::mat4 get_view_matrix() const;
    glm::vec3 get_forward_vector() const;
    glm::vec3 get_up_vector() const;

    math::vec3 get_forward_vector_math () const;
    math::vec3 get_up_vector_math      () const;
};
