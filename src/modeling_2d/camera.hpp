#pragma once

#include <glm/glm.hpp>

struct Camera2D {
    glm::vec3 position;

    Camera2D();

    glm::mat4 get_view_matrix();
};
