#include "camera.hpp"
#include <logging/logging.h>
#include <core/input.hpp>
#include <glm/gtc/matrix_transform.hpp>


constexpr f32 CAMERA_SPEED = 2.f;
constexpr f32 ZOOM_SPEED = .1f;


Camera2D::Camera2D()
    : position{0.f, 0.f, -15.f}
{}


glm::mat4 Camera2D::get_view_matrix() const {
    glm::vec3 up = glm::vec3(0.f, 1.f, 0.f);

    glm::vec3 direction;
    math::vector3 forward = get_forward_vector();
    direction.x = forward.x;
    direction.y = forward.y;
    direction.z = forward.z;

    glm::vec3 glm_position;
    glm_position.x = position.x;
    glm_position.y = position.y;
    glm_position.z = position.z;

    return glm::lookAt(glm_position, glm_position + direction, up);
}


math::mat4 Camera2D::get_view_matrix_math() const {
    glm::mat4 result = get_view_matrix();

    return {
        result[0][0], result[0][1], result[0][2], result[0][3],
        result[1][0], result[1][1], result[1][2], result[1][3],
        result[2][0], result[2][1], result[2][2], result[2][3],
        result[3][0], result[3][1], result[3][2], result[3][3]
    };
}


math::vector3 Camera2D::get_forward_vector () const {
    return { 0.f, 0.f, 1.f };
}


math::vector3 Camera2D::get_up_vector () const {
    return { 0.f, 1.f, 0.f };
}
