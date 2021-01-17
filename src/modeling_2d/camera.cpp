#include "camera.hpp"
#include <logging/logging.h>
#include <es/event_system.h>
#include <api/keyboard.h>
#include <glm/gtc/matrix_transform.hpp>


constexpr f32 CAMERA_SPEED = .5f;

void update(Camera2D *camera, f32 dt) {
    bool changed = false;
    FOR_ALL_KEYS(k) {
        if (Keyboard::is_pressed(k)) {
            switch (k) {
                case Keyboard::W: camera->position.y += CAMERA_SPEED*dt; changed = true; break;
                case Keyboard::S: camera->position.y -= CAMERA_SPEED*dt; changed = true; break;
                case Keyboard::A: camera->position.x += CAMERA_SPEED*dt; changed = true; break;
                case Keyboard::D: camera->position.x -= CAMERA_SPEED*dt; changed = true; break;
                default: break;
            }
        }
    }

    // if (changed) {
    //     LOG_DEBUG << "Camera { x: " << camera->position.x << ", y : " << camera->position.y << " };";
    // }
}

Camera2D::Camera2D() 
    : position(glm::vec3(0.f, 0.f, -1.f))
{
    Dispatcher<EventFrameFinished>::subscribe([this](EventFrameFinished e){ 
        update(this, e.dt);
    });
    Dispatcher<EventZoom>::subscribe([this](EventZoom e) {
        auto z = this->position.z + e.factor;
        if (z > -1.f) z = -1.f;
        this->position.z = z;
    });
}

glm::mat4 Camera2D::get_view_matrix() const {
    glm::vec3 up = glm::vec3(0.f, 1.f, 0.f);
    glm::vec3 direction = get_forward_vector();
    return glm::lookAt(position, position + direction, up);
}

glm::vec3 Camera2D::get_forward_vector() const {
    return glm::vec3(0.f, 0.f, 1.f);
}

glm::vec3 Camera2D::get_up_vector() const {
    return glm::vec3(0.f, 1.f, 0.f);
}

math::vec3 Camera2D::get_forward_vector_math () const {
    return math::vec3(0.f, 0.f, 1.f);
}

math::vec3 Camera2D::get_up_vector_math () const {
    return math::vec3(0.f, 1.f, 0.f);
}
