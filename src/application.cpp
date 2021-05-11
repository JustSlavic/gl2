#include <application.h>
#include <keymap.h>
#include <api/mouse.h>
#include <api/keyboard.h>
#include <iostream>
#include <thread>
#include <chrono>

#include <config.hpp>

//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>

#include <math/vector3.hpp>
#include <fs/watcher.h>
#include <graphics/graphics_api.h>
#include <graphics/shader.h>
#include <graphics/vertex_array.h>
#include <graphics/vertex_buffer.h>
#include <graphics/index_buffer.h>
#include <graphics/renderer.h>

#include <modeling_2d/camera.hpp>
#include <modeling_2d/model.hpp>
#include <modeling_2d/creatures.hpp>

#include <service/shader_library.hpp>


#define GRAVITY


constexpr f32 NEAR_CLIP_DISTANCE = 0.1f;
constexpr f32 FAR_CLIP_DISTANCE = 1000.f;

namespace gl2 {
    void print_matrix4(math::matrix4 m) {
        printf("|%5.2f, %5.2f, %5.2f, %5.2f|\n"
               "|%5.2f, %5.2f, %5.2f, %5.2f|\n"
               "|%5.2f, %5.2f, %5.2f, %5.2f|\n"
               "|%5.2f, %5.2f, %5.2f, %5.2f|\n",
            m._11, m._12, m._13, m._14,
            m._21, m._22, m._23, m._24,
            m._31, m._32, m._33, m._34,
            m._41, m._42, m._43, m._44
        );
    }

    math::vector3 intersect_z0_plane (math::vector3 ray_start, math::vector3 vector) {
        return intersect_plane(ray_start, vector, math::vector3::make(0.f), math::vector3::make(0.f, 0.f, 1.f));
    }

    // Instantiating template BEFORE first usage
    Application::Application() {
        Dispatcher<EventStop>::subscribe(EVENT_CALLBACK(on_stop));
        // Dispatcher<Keyboard::KeyPressEvent>::subscribe([](Keyboard::KeyPressEvent event) {
            // LOG_DEBUG << "Keyboard{ " << to_string(event.key) << " };";
        // });
    }

    Application::~Application() {
        window->shutdown();
        delete window;

        GraphicsApi::shutdown();
    }

    int Application::initialize() {
        if (window) return 1;

        bool config_initialized = config::initialize("config.son");
        if (not config_initialized) {
            printf("Could not initialize config!\n");
            return 1;
        }

        auto& cfg = config::get_instance();
        LOG_INFO << "WINDOW: " << cfg.window.width << "x" << cfg.window.height;

        window = new Window(cfg.window.width, cfg.window.height);

        i32 err = window->startup();
        if (err) return err;

        Keymap::instance();
        GraphicsApi::startup();
        Renderer::init();

        return 0;
    }

    int Application::run() {
        running = true;

        LOG_DEBUG << "Graphics API: " << to_string(GraphicsApi::get_api_name());

        i32 w = window->get_width();
        i32 h = window->get_height();

        f32 window_ratio = f32(w) / f32(h);

        // glm::mat4 projection = glm::ortho(-(f32)w*.5f/h, (f32)w*.5f/h, -.5f, .5f);
        auto projection = math::projection(window_ratio * NEAR_CLIP_DISTANCE, NEAR_CLIP_DISTANCE, NEAR_CLIP_DISTANCE, FAR_CLIP_DISTANCE);


        printf("projection matrix:\n");
        print_matrix4(projection);
        printf("\n");


#ifdef GRAVITY
        Model model;
#endif

#ifdef CREATURES
        simulation model;
#endif

        Camera2D camera;
        printf("view matrix:\n");
        print_matrix4(camera.get_view_matrix_math());
        printf("\n");

        Dispatcher<Mouse::MoveEvent>::subscribe([&](auto event) {
            math::vector2 mouse{ (f32(event.x) / f32(w) - .5f), (-f32(event.y) / f32(h) + .5f) };

            math::vector3 position = camera.position;

            math::vector3 forward = camera.get_forward_vector();
            math::vector3 up = camera.get_up_vector();
            math::vector3 right = math::cross(forward, up);

            f32 clip_width = NEAR_CLIP_DISTANCE * window_ratio;
            f32 clip_height = NEAR_CLIP_DISTANCE;

            math::vector3 center = position + forward * NEAR_CLIP_DISTANCE;
            math::vector3 point = center + right * mouse.x * clip_width + up * mouse.y * clip_height;
            math::vector3 direction = point - position;

            math::vector3 intersection = intersect_z0_plane(position, direction);

            printf("mouse position: (%5.2f, %5.2f, %5.2f)\n", intersection.x, intersection.y, intersection.z);

            model.on_mouse_move(intersection.xy);
        });

        Dispatcher<Mouse::ButtonPressEvent>::subscribe([&] (Mouse::ButtonPressEvent e) {
            if (e.button == Mouse::LEFT) {
                auto& m = Mouse::instance();
                // printf("==========================\n");

                math::vector3 position = camera.position;
                // printf("position = (%5.2f, %5.2f, %5.2f)\n", position.x, position.y, position.z);


                math::vector3 forward = camera.get_forward_vector();
                math::vector3 up = camera.get_up_vector();
                math::vector3 right = math::cross(forward, up);

                // f32 t = 2 * NEAR_CLIP_DISTANCE * ::std::tan(.5f * math::radians(45.f));
                // f32 clip_width = t * window_ratio;
                // f32 clip_height = t;

                f32 clip_width = NEAR_CLIP_DISTANCE * window_ratio;
                f32 clip_height = NEAR_CLIP_DISTANCE;

                // printf("NEAR_CLIP{ WIDTH = %5.2f; HEIGHT = %5.2f; }\n", clip_width, clip_height);

                math::vector2 mouse = {
                    ( f32(m.x) / f32(w) - .5f),
                    (-f32(m.y) / f32(h) + .5f)
                };
                // printf("mouse = (%5.2f, %5.2f)\n", mouse.x, mouse.y);

                math::vector3 center = position + forward * NEAR_CLIP_DISTANCE;
                math::vector3 point = center + right*mouse.x * clip_width + up*mouse.y * clip_height;
                // printf("point = (%5.2f, %5.2f, %5.2f)\n", point.x, point.y, point.z);

                math::vector3 direction = point - position;

                math::vector3 intersection = intersect_z0_plane(position, direction);

                printf("click position: (%5.2f, %5.2f, %5.2f)\n", intersection.x, intersection.y, intersection.z);

                math::mat4 model_ = math::mat4::eye();
                math::mat4 view_ = camera.get_view_matrix_math();

                math::vector4 q = math::vector4::make(intersection, 0.f);

                q = model_ * q;
                //printf("in space: q = (%5.2f, %5.2f, %5.2f, %5.2f)\n", q.x, q.y, q.z, q.w);
                q = q * view_;
                //printf("in camera: q = (%5.2f, %5.2f, %5.2f, %5.2f)\n", q.x, q.y, q.z, q.w);
                q = projection * q;
                //printf("in screen: q = (%5.2f, %5.2f, %5.2f, %5.2f)\n", q.x, q.y, q.z, q.w);

                q = q / q.w;
                //printf("normalized: q = (%5.2f, %5.2f, %5.2f, %5.2f)\n", q.x, q.y, q.z, q.w);

#ifdef GRAVITY
                model.on_mouse_click(intersection.xy);
#endif
#ifdef CREATURES
                model.add_creature(creature::make_random(math::vector2(intersection.x, intersection.y)));
#endif
            }
        });

        Dispatcher<EventSelectedBodyMoved>::subscribe([&](EventSelectedBodyMoved e) {
            camera.position.xy = e.body_position;
        });

        f32 vertices[] = {
             0.5f,  0.5f, 0.0f,  // top right
             0.5f, -0.5f, 0.0f,  // bottom right
            -0.5f, -0.5f, 0.0f,  // bottom left
            -0.5f,  0.5f, 0.0f   // top left
        };

        u32 indices[] = {  // note that we start from 0!
            0, 1, 3,  // first Triangle
            1, 2, 3   // second Triangle
        };

        auto body_shader = Shader();
        body_shader.load_shader(Shader::Type::Vertex, "resources/shaders/body.vshader")
            .load_shader(Shader::Type::Fragment, "resources/shaders/body.fshader")
            .compile()
            .bind();

        auto arrow_shader = Shader();
        arrow_shader.load_shader(Shader::Type::Vertex, "resources/shaders/arrow.vshader")
            .load_shader(Shader::Type::Fragment, "resources/shaders/arrow.fshader")
            .compile();

        body_shader.set_uniform_3f("u_color", math::color24::make(1.f));
        body_shader.set_uniform_mat4f("u_projection", projection);
        arrow_shader.set_uniform_mat4f("u_projection", projection);

        VertexArray va;
        va.bind();

        VertexBuffer vb(vertices, sizeof(vertices)); // here should be size
        IndexBuffer ib(indices, sizeof(indices) / sizeof(u32)); // here should be count of elements
        VertexBufferLayout vbl;
        vbl.push<f32>(3);

        va.add_buffer(vb, vbl);

        model.shader = &body_shader;
        model.va = &va;
        model.ib = &ib;
#ifdef GRAVITY
        model.arrow_shader = &arrow_shader;
#endif

        auto t = std::chrono::steady_clock::now();
        std::this_thread::sleep_for(std::chrono::milliseconds(16));

        auto& cfg = config::get_instance();
        math::color24 background_color{ cfg.window.default_color.r, cfg.window.default_color.g, cfg.window.default_color.b };
        // uncomment this call to draw in wireframe polygons.
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        u64 frame_counter = 0;

        while (running) {
            auto t_ = std::chrono::steady_clock::now();
            auto dt = std::chrono::duration_cast<std::chrono::microseconds>(t_ - t).count();
            //printf("%llu: dt = %ld microseconds; fps = %lf\n", frame_counter++, dt, 1000000.0 / dt);
            t = t_;

            auto view = camera.get_view_matrix_math();

            body_shader.set_uniform_mat4f("u_view", view);
            body_shader.set_uniform_mat4f("u_model", math::mat4::eye());

            arrow_shader.set_uniform_mat4f("u_view", view);
            arrow_shader.set_uniform_mat4f("u_model", math::mat4::eye());

            Renderer::clear(background_color);
            // Renderer::draw(va, ib, shdr);

            // Draw Ox
            {
                /*arrow_shader.set_uniform_mat4f("u_model", glm::translate(glm::scale(glm::mat4(1.f), glm::vec3(0.1f)), glm::vec3(.5f, 0.f, 0.f)));
                arrow_shader.set_uniform_3f("u_color", 0.f, 0.f, 1.f);
                arrow_shader.bind();
                Renderer::draw(va, ib, arrow_shader);*/
            }

            // Draw Oy
            {
                /*arrow_shader.set_uniform_mat4f("u_model", glm::translate(glm::rotate(glm::scale(glm::mat4(1.f), glm::vec3(0.1f)), glm::radians(90.f), glm::vec3(0.f, 0.f, 1.f)), glm::vec3(.5f, 0.f, 0.f)));
                arrow_shader.set_uniform_3f("u_color", 1.f, 0.f, 0.f);
                arrow_shader.bind();
                Renderer::draw(va, ib, arrow_shader);*/
            }

            arrow_shader.set_uniform_3f("u_color", 1.f, 1.f, 0.f);

#ifdef GRAVITY
            model.draw_bodies();
#endif
#ifdef CREATURES
            model.iterate();
#endif

            window->poll_events();
            window->swap_buffers();

            emit<EventFrameFinished>(dt / 1000000.0f);
            // std::this_thread::sleep_for(std::chrono::milliseconds(16));
        }

        return 0;
    }

    void Application::on_stop(EventStop) {
        LOG_INFO << "Received StopEvent";
        running = false;
    }
}

