#include "model.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <es/event_system.h>
#include <api/mouse.h>
#include <graphics/renderer.h>


inline f32 mass_to_radius(f32 m) {
    constexpr f32 rho = 10000.f;
    return std::sqrt(m / (M_PI * rho));
}


body::body(f32 x, f32 y)
    : position(x, y)
    , vx(0.f)
    , vy(0.f)
    , m(1.f)
{}

body::body(const math::vec2& p)
    : position(p)
    , vx(0.f)
    , vy(0.f)
    , m(1.f)
{}

body::body(f32 x, f32 y, f32 vx, f32 vy, f32 m)
    : position(x, y)
    , vx(vx)
    , vy(vy)
    , m(m)
{}

body::body(const math::vec2& p, f32 vx, f32 vy, f32 m)
    : position(p)
    , vx(vx)
    , vy(vy)
    , m(m)
{}

std::vector<body> generate_bodies(f32 width, f32 height);
Model::Model()
    // : bodies(malloc())
{
    Dispatcher<EventFrameFinished>::subscribe([this] (EventFrameFinished e) {
        this->move_bodies(e.dt);
    });
    Dispatcher<EventRestart>::subscribe([this](EventRestart) { this->clean(); });
    Dispatcher<EventPause>::subscribe([this](EventPause) { this->pause = not this->pause; });
    Dispatcher<EventToggleF2>::subscribe([this](EventToggleF2) { this->elastic = not this->elastic; });

    bodies.reserve(5000);
    buffer.reserve(5000);

    // bodies = generate_bodies(3.f, 3.f);
}

void Model::add_body(f32 x, f32 y) {
    LOG_DEBUG << "Body added at (" << x << ", " << y << ");";
    auto b = body(x, y);
    b.m = 3.f;
    bodies.emplace_back(b);
}


void Model::draw_bodies() {
    for (auto& b : bodies) {
        // drawing the body
        auto r = mass_to_radius(b.m);
        
        {
            auto model_matrix = 
                glm::scale(
                    glm::translate(
                        glm::mat4(1.f), 
                        glm::vec3(b.position.x, b.position.y, 0.f)
                    ),
                    glm::vec3(r * 2.f)
                );

            shader->set_uniform_mat4f("u_model", model_matrix);
            shader->bind();
            gl2::Renderer::draw(*va, *ib, *shader);
        }

        {
            auto velocity = glm::vec2(b.vx, b.vy);
            f32 v = glm::length(velocity);
            f32 angle = std::atan2(velocity.y, velocity.x);

            auto model_matrix =
                glm::rotate(
                    glm::translate(
                        glm::scale(
                            glm::translate(
                                glm::mat4(1.f),
                                glm::vec3(b.position.x, b.position.y, 0.f)
                            ),
                            glm::vec3(v)
                        ), 
                        glm::normalize(glm::vec3(velocity.x, velocity.y, 0.f)) * 0.5f
                    ),
                    angle,
                    glm::vec3(0.f, 0.f, 1.f)
                );

            // drawing the arrows
            arrow_shader->set_uniform_mat4f("u_model", model_matrix);
            // arrow_shader->set_uniform_1f("u_length", 1.f);
            // arrow_shader->set_uniform_1f("u_width", 1.f);
            arrow_shader->bind();
            gl2::Renderer::draw(*va, *ib, *arrow_shader);
        }
    }
}

std::vector<body> generate_bodies(f32 width, f32 height) {
    std::vector<body> bodies;
    auto radius = 1.f;

    for (f32 x = - width * .5f; x < width * .5f; x += .1f) {
        for (f32 y = - height * .5f; y < height * .5f; y += .1f) {
            if (glm::length(glm::vec2(x, y)) < radius) {
                bodies.emplace_back(x, y, - y * .1f, x * .1f, 1.f);
            }
        }
    }

    for (f32 phi = 0; phi < 2.f * M_PI; phi += M_PI * .1f) {
        for (f32 r = .1f; r < radius; r += .1f) {
            f32 x = r*std::cos(phi);
            f32 y = r*std::sin(phi);

            bodies.emplace_back(x, y, - y * .1f, x * .1f, 1.f);
        }
    }

    return bodies;
}

void interact_inelastic(std::vector<body>& bodies, std::vector<body>& buffer) {
    buffer.clear();
    std::vector<bool> merged(bodies.size(), false);

    for (size_t i = 0; i < bodies.size(); i++) {
        if (merged[i]) continue;
        auto a = bodies[i];

        for (size_t j = 0; j < bodies.size(); j++) {
            if (merged[i]) continue;
            if (i == j) continue;
            auto &b = bodies[j];

            f32 d = (b.position - a.position).length();
            if (d < (mass_to_radius(a.m) + mass_to_radius(b.m))) {
                merged[j] = true;

                a.position = (a.position * a.m + b.position * b.m) / (a.m + b.m);
                a.vx = (a.vx*a.m + b.vx*b.m) / (a.m + b.m);
                a.vy = (a.vy*a.m + b.vy*b.m) / (a.m + b.m);

                a.m += b.m;
            }
        }

        buffer.push_back(a);
    }

    std::swap(bodies, buffer);
}

void interact_elastic(std::vector<body>& bodies, std::vector<body>& buffer) {
    buffer.clear();
    std::vector<bool> interacted(bodies.size(), false);

    for (size_t i = 0; i < bodies.size(); i++) {
        if (interacted[i]) continue;
        auto a = bodies[i];

        for (size_t j = i+1; j < bodies.size(); j++) {
            if (interacted[j]) continue;
            auto b = bodies[j];

            f32 d = (b.position - a.position).length();

            if (d < (mass_to_radius(a.m) + mass_to_radius(b.m))) {
                interacted[i] = true;
                interacted[j] = true;

                auto a_v = math::vec2(a.vx, a.vy);
                auto b_v = math::vec2(b.vx, b.vy);

                math::vec2 a_direction = -math::normalize(a_v);
                // auto b_direction = -glm::normalize(b_v);

                f32 av = math::length(a_v);
                f32 bv = math::length(b_v);

                auto new_av = av*(a.m - b.m)/(a.m + b.m) + 2.f*bv*b.m/(a.m + b.m);
                // auto new_bv = bv*(b.m - a.m)/(a.m + b.m) + 2.f*av*a.m/(a.m + b.m);
                
                a.vx = a_direction.x * new_av;
                a.vy = a_direction.y * new_av;
                // b.vx = b_direction.x * new_bv;
                // b.vy = b_direction.y * new_bv;

                buffer.push_back(b);
            }
        }

        buffer.push_back(a);
    }

    std::swap(bodies, buffer);
}

void Model::move_bodies(f32 dt) {
    if (pause) return;

    if (elastic) {
        interact_elastic(bodies, buffer);
    } else {
        interact_inelastic(bodies, buffer);
    }

    buffer.clear();

    for (size_t i = 0; i < bodies.size(); i++) {
        auto& b = bodies[i];

        math::vec2 new_position = b.position + math::vec2(b.vx, b.vy) * dt;

        f32 new_vx = b.vx;
        f32 new_vy = b.vy;

        constexpr f32 G = .00001;

        for (auto& other : bodies) {
            math::vec2 dr = other.position - b.position;
            f32 dr_2 = dr.length_2();
            f32 dr_1 = math::sqrt(dr_2);

            if (math::equal(dr_1, 0.f)) continue;

            f32 f = G * other.m / (dr_2 * dr_1);
            new_vx += dt * dr.x * f;
            new_vy += dt * dr.y * f;
        }

        buffer.emplace_back(new_position, new_vx, new_vy, b.m);
    }

    std::swap(bodies, buffer);
}

void Model::clean() {
    bodies.clear();
}
