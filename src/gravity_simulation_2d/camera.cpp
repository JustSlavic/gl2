#include "camera.hpp"
#include <logging/logging.h>
#include <core/input.hpp>
//#include <glm/glm.hpp>
//#include <glm/ext/matrix_transform.hpp>

constexpr f32 CAMERA_SPEED = 2.f;
constexpr f32 ZOOM_SPEED = .1f;


Camera2D::Camera2D()
    : position{0.f, 0.f, -15.f}
{}


math::matrix4 Camera2D::get_view_matrix() const {
    auto up = math::vector3{ 0.f, 1.f, 0.f };
    auto direction = get_forward_vector();

    //auto glm_position = glm::vec3(position.x, position.y, position.z);
    //auto glm_direction = glm::vec3(direction.x, direction.y, direction.z);
    //auto glm_up = glm::vec3(0, 1, 0);

    //auto look_at_matrix = glm::lookAt(glm_position, glm_position + glm_direction, glm_up);
    auto look_at_result = math::look_at(position, position + direction, up);

    return look_at_result;
}


math::vector3 Camera2D::get_forward_vector () const {
    return { 0.f, 0.f, 1.f };
}


math::vector3 Camera2D::get_up_vector () const {
    return { 0.f, 1.f, 0.f };
}
