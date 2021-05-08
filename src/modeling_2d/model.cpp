#include "model.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <es/event_system.h>
#include <api/mouse.h>
#include <graphics/renderer.h>

#include <random>
#include <thread>



constexpr f32 G = .00001f;


inline f32 mass_to_radius(f32 m) {
    constexpr f32 rho = 1000000.f;
    //return std::sqrt(m / (math::consts<f32>::pi() * rho));
    return std::cbrtf(3.f * m / (4.f * rho * math::consts<f32>::pi()));
}


body::body(f32 x, f32 y)
    : position{x, y}
    , velocity{0.f, 0.f}
    , m(1.f)
{}

body::body(const math::vector2& p)
    : position{p}
    , velocity{0.f, 0.f}
    , m(1.f)
{}

body::body(f32 x, f32 y, f32 vx, f32 vy, f32 m)
    : position{x, y}
    , velocity{ vx, vy }
    , m(m)
{}

body::body(math::vector2 p, math::vector2 v, f32 m)
    : position{p}
    , velocity{v}
    , m(m)
{}


void generate_bodies(Model* model) {
    std::random_device random_device;
    std::mt19937 gen(random_device());
    std::uniform_real_distribution<f32> distribution(0.f, 1.f);

    auto inner_r = 1.f;
    auto outer_r = inner_r + 8.f;
    f32 velocity_multiplier = 2.0f;
    //f32 M = 20000.f;
    f32 M = 40000.f;

    model->add_body(math::vector2{ 0.f }, math::vector2{ 0.f }, M);

    for (f32 x = -outer_r * 2.f; x < outer_r * 2.f; x += .1f) {
        for (f32 y = -outer_r; y < outer_r * 2.f; y += .1f) {
            if (distribution(gen) < 0.9f) continue; // Introduce a little bit of random

            f32 len = math::vector2{ x, y }.length();
            if (inner_r < len and len < outer_r) {
                auto v = math::vector2{ -y, x }.normalized();
                model->add_body(math::vector2{ x, y }, v * math::sqrt(G * M / len) * 0.98f, distribution(gen) * 5.f);
            }
        }
    }
}


void generate_random_bodies_in_square(Model* model, f32 square_size = 1.f) {
    std::random_device random_device;
    std::mt19937 gen(random_device());
    std::uniform_real_distribution<f32> distribution(0.f, 1.f);

    for (f32 x = -square_size * .5f; x < square_size * .5f; x += .1f) {
        for (f32 y = -square_size * .5f; y < square_size * .5f; y += .1f) {
            if (distribution(gen) < 0.9f) continue; // Introduce a little bit of random

            math::vector2 position{ x, y };
            math::vector2 velocity{ distribution(gen) - 0.5f, distribution(gen) - 0.5f };
            f32 mass = distribution(gen) * 5.f;
            model->add_body(position, velocity * 0.05f, mass);
        }
    }
}


Model::Model()
{
    Dispatcher<EventFrameFinished>::subscribe([this] (EventFrameFinished e) {
        this->move_bodies(e.dt);
    });
    Dispatcher<EventRestart>::subscribe([this](EventRestart) { this->clean(); });
    Dispatcher<EventPause>::subscribe([this](EventPause) { this->pause = not this->pause; });
    Dispatcher<EventToggleF2>::subscribe([this](EventToggleF2) { this->elastic = not this->elastic; });
    Dispatcher<EventToggleBodyTraces>::subscribe([this](EventToggleBodyTraces) { this->toggle_body_traces(); });

    bodies.reserve(5000);
    bodies_buffer.reserve(5000);

    radii.reserve(5000);
    radii_buffer.reserve(5000);

    traces.reserve(5000);
    traces_buffer.reserve(5000);

    generate_bodies(this);
    //generate_random_bodies_in_square(this, 0.8f);
}


void Model::add_body(math::vector2 position, math::vector2 velocity, f32 mass) {
    // printf("Body added at (%5.2f, %5.2f)\n", x, y);
    bodies.emplace_back(position, velocity, mass);
    radii.emplace_back(mass_to_radius(mass));
    traces.push_back({});
}


void Model::draw_bodies() {
    //printf("size = %llu\n", bodies.size());
    for (size_t i = 0; i < bodies.size(); i++) {
        const body& b = bodies[i];
        // drawing the body
        {
            auto model_matrix = glm::mat4(1.f);
            model_matrix = glm::translate(model_matrix, glm::vec3(b.position.x, b.position.y, 0.f));
            model_matrix = glm::scale(model_matrix, glm::vec3(radii[i] * 2.f));

            shader->set_uniform_mat4f("u_model", model_matrix);
            shader->bind();

            gl2::Renderer::draw(*va, *ib, *shader);
        }

        //{
        //   f32 v = b.velocity.length();
        //   f32 angle = std::atan2(b.velocity.y, b.velocity.x);

        //   auto model_matrix = glm::mat4(1.f);
        //   model_matrix = glm::translate(model_matrix, glm::vec3(b.position.x, b.position.y, 0.f));
        //   model_matrix = glm::rotate(model_matrix, angle, glm::vec3(0.f, 0.f, 1.f));
        //   model_matrix = glm::translate(model_matrix, glm::vec3(v * 0.5f, 0.f, 0.f));
        //   model_matrix = glm::scale(model_matrix, glm::vec3(v, 0.002f, 0.f));

        //   // drawing the arrows
        //   arrow_shader->set_uniform_mat4f("u_model", model_matrix);
        //   arrow_shader->bind();
        //   gl2::Renderer::draw(*va, *ib, *arrow_shader);
        //}

        if (draw_body_traces) {
            const std::vector<math::vector2>& trace = traces[i];
            for (size_t t = 0; t + 1 < trace.size(); t++) {
                auto p0 = trace[t];
                auto p1 = trace[t + 1];

                auto dr = p1 - p0;
                f32 angle = std::atan2(dr.y, dr.x);
                f32 v = dr.length();

                auto model_matrix = glm::mat4(1.f);
                model_matrix = glm::translate(model_matrix, glm::vec3(p0.x, p0.y, 0.f));
                model_matrix = glm::rotate(model_matrix, angle, glm::vec3(0.f, 0.f, 1.f));
                model_matrix = glm::translate(model_matrix, glm::vec3(v * 0.5f, 0.f, 0.f));
                model_matrix = glm::scale(model_matrix, glm::vec3(v, 0.006f, 0.f));

                arrow_shader->set_uniform_mat4f("u_model", model_matrix);
                arrow_shader->bind();
                gl2::Renderer::draw(*va, *ib, *arrow_shader);
            }
        }
    }
}


void interact_inelastic(Model* model) {
    model->bodies_buffer.clear();
    model->radii_buffer.clear();
    model->traces_buffer.clear();

    size_t n = model->bodies.size();

    std::vector<bool> merged(n, false);

    for (size_t i = 0; i < n; i++) {
        if (merged[i]) continue;
        body a = model->bodies[i];

        for (size_t j = 0; j < n; j++) {
            if (merged[j]) continue;
            if (i == j) continue;
            auto &b = model->bodies[j];

            f32 d_2 = (b.position - a.position).length_2();
            f32 r = model->radii[i] + model->radii[j];

            if (d_2 < r*r) {
                merged[j] = true;

                a.position = (a.position * a.m + b.position * b.m) / (a.m + b.m);
                a.velocity = (a.velocity * a.m + b.velocity * b.m) / (a.m + b.m);

                a.m += b.m;
            }
        }

        model->bodies_buffer.push_back(a);
        model->radii_buffer.push_back(mass_to_radius(a.m));
        if (model->draw_body_traces) model->traces_buffer.push_back(model->traces[i]);
    }

    std::swap(model->bodies, model->bodies_buffer);
    std::swap(model->radii, model->radii_buffer);
    std::swap(model->traces, model->traces_buffer);
}

//void interact_elastic(std::vector<body>& bodies, std::vector<body>& buffer) {
//    buffer.clear();
//    std::vector<bool> interacted(bodies.size(), false);
//    std::vector<f32> radii(bodies.size(), 0.f);
//
//    for (size_t i = 0; i < bodies.size(); i++) {
//        if (interacted[i]) continue;
//        auto a = bodies[i];
//
//        for (size_t j = i+1; j < bodies.size(); j++) {
//            if (interacted[j]) continue;
//            const auto& b = bodies[j];
//
//            f32 d_2 = (b.position - a.position).length_2();
//            if (radii[i] < 0.01f) radii[i] = mass_to_radius(a.m);
//            if (radii[j] < 0.01f) radii[j] = mass_to_radius(b.m);
//            f32 r = radii[i] + radii[j];
//            if (d_2 < r*r) {
//                interacted[i] = true;
//                interacted[j] = true;
//
//                math::vector2 a_direction = -math::normalize(a.velocity);
//                // auto b_direction = -glm::normalize(b_v);
//
//                f32 av = math::length(a.velocity);
//                f32 bv = math::length(b.velocity);
//
//                auto new_av = av*(a.m - b.m)/(a.m + b.m) + 2.f*bv*b.m/(a.m + b.m);
//                // auto new_bv = bv*(b.m - a.m)/(a.m + b.m) + 2.f*av*a.m/(a.m + b.m);
//
//                a.velocity = a_direction * new_av;
//
//                // b.vx = b_direction.x * new_bv;
//                // b.vy = b_direction.y * new_bv;
//
//                buffer.push_back(b);
//            }
//        }
//
//        buffer.push_back(a);
//    }
//
//    std::swap(bodies, buffer);
//}

void Model::move_bodies(f32 dt) {
    if (pause) return;

    if (elastic) {
        //interact_elastic(bodies, buffer);
    } else {
        interact_inelastic(this);
    }

    //dt = 0.01f;
    bodies_buffer.clear();
    radii_buffer.clear();
    traces_buffer.clear();

    auto run_move_from_to = [&](size_t from, size_t to) {
        for (size_t i = from; i < to; i++) {
            auto& b = bodies[i];

            math::vector2 new_position = b.position + b.velocity * dt;
            math::vector2 new_velocity = b.velocity;

            for (size_t j = 0; j < bodies.size(); j++) {
                if (i == j) continue;

                auto& other = bodies[j];

                math::vector2 dr = other.position - b.position;
                f32 dr_2 = dr.length_2();
                f32 dr_1 = math::sqrt(dr_2);

                f32 f = G * other.m / (dr_2 * dr_1);
                new_velocity += dt * dr * f;
            }

            if (new_position.length_2() > 100.f) continue;

            bodies_buffer.emplace_back(new_position, new_velocity, b.m);
            radii_buffer.push_back(mass_to_radius(b.m));

            if (draw_body_traces) {
                //if (traces[i].size() > 50) traces[i].erase(traces[i].begin());
                traces[i].push_back(new_position);
                traces_buffer.push_back(traces[i]);
            }
        }
    };

    size_t n = bodies.size();
    //std::thread worker_1{run_move_from_to, 0, n / 3};
    //std::thread worker_2{ run_move_from_to, n / 3, 2 * n / 3 };
    //run_move_from_to(2 * n / 3, n);
    run_move_from_to(0, n);
    //worker_1.join();
    //worker_2.join();

    std::swap(bodies, bodies_buffer);
    std::swap(radii, radii_buffer);
    std::swap(traces, traces_buffer);
}

void Model::clean() {
    bodies.clear();
    radii.clear();
    traces.clear();
    generate_bodies(this);
    //generate_random_bodies_in_square(this);
}

void Model::toggle_body_traces() {
    if (draw_body_traces) {
        traces.clear();
        traces_buffer.clear();
    }

    draw_body_traces = not draw_body_traces;
}
