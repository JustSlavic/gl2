#include "model.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <graphics/renderer.h>


namespace physics_simulation_2d {

float32 GRAVITY_ACCELERATION = 9.8; // [m/s^2]


model::model() {
    dynamic_stuff.push_back(physics::dynamic_body_2d({ 0, 0 }, { 0, 0 }, 0.1));
    dynamic_stuff.push_back(physics::dynamic_body_2d({ 0.5, 0 }, { 0, 0 }, 0.1));
    dynamic_stuff.push_back(physics::dynamic_body_2d({ 0.2, 0.3 }, { 0, 0 }, 0.1));

    static_stuff.push_back(physics::static_segment_2d({ -2, -1 }, { 2, -1 }));
}


void model::draw() {
    u32 draw_calls = 0;

    arrow_shader->bind();
    gl2::Renderer::draw(*va, *ib, *arrow_shader);
    draw_calls++;

    for (auto& segment : static_stuff) {
        auto model_matrix = glm::mat4(1.f);
        model_matrix = glm::translate(model_matrix,
            glm::vec3((segment.start.x + segment.end.x) / 2.f, (segment.start.y + segment.end.y) / 2.f, 0.f)
        );
        model_matrix = glm::scale(model_matrix, glm::vec3(math::length(segment.start - segment.end), 0.1f, 1.f));
        arrow_shader->set_uniform_mat4f("u_model", model_matrix);

        gl2::Renderer::draw(*va, *ib, *arrow_shader);
        draw_calls++;
    }

    for (auto& body : dynamic_stuff) {
        shader->set_uniform_3f("u_color", math::color24{ 1.f, 1.f, 1.f });

        auto model_matrix = glm::mat4(1.f);
        model_matrix = glm::translate(model_matrix, glm::vec3(body.position.x, body.position.y, 0.f));
        model_matrix = glm::scale(model_matrix, glm::vec3(0.1f));

        shader->set_uniform_mat4f("u_model", model_matrix);
        shader->bind();

        gl2::Renderer::draw(*va, *ib, *shader);
        draw_calls++;
    }

    //printf("draw_calls: %u\n", draw_calls);
}


void model::advance(f32 dt) {
    for (auto& body : dynamic_stuff) {
        body.position += body.velocity * dt;
        body.velocity += math::vector2{ 0, -1 } * GRAVITY_ACCELERATION * dt;

        for (auto& segment : static_stuff) {
            auto w = segment.end - segment.start;
            auto t = - w.x* (segment.start.y - body.position.y) + w.y * (segment.start.x - body.position.x);
            
            if (body.position.y - 0.05 < segment.start.y + 0.05) {
                body.position.y = segment.start.y + 0.1f;

                f32 elasticity = 0.5f;
                body.velocity = body.velocity - (1 + elasticity) * segment.get_normal() * math::dot(body.velocity, segment.get_normal());

                //body.velocity = math::reflect(body.velocity, segment.get_normal()) * 0.8;
            }
        }

    }
}


} // physics_simulation_2d
