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
                case Keyboard::A: camera->position.x -= CAMERA_SPEED*dt; changed = true; break;
                case Keyboard::D: camera->position.x += CAMERA_SPEED*dt; changed = true; break;
                default: break;
            }
        }
    }

    if (changed) {
        LOG_DEBUG << "Camera { x: " << camera->position.x << ", y : " << camera->position.y << " };";
    }
}

Camera2D::Camera2D() 
    : position(glm::vec3(0.f, 0.f, 1.f))
{
    Dispatcher<EventFrameFinished>::subscribe([this](EventFrameFinished e){ 
        update(this, e.dt);
    });
    Dispatcher<EventZoom>::subscribe([this](EventZoom e) {
        auto z = this->position.z + e.factor;
        if (z < 1.f) z = 1.f;
        this->position.z = z;
    });
}

glm::mat4 Camera2D::get_view_matrix() {
    glm::vec3 up = glm::vec3(0.f, 1.f, 0.f);
    glm::vec3 direction = glm::vec3(0.f, 0.f, -1.f);
    return glm::lookAt(position, position + direction, up);
}

