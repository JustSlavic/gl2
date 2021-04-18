#include <application.h>
#include <keymap.h> 
#include <api/mouse.h>
#include <api/keyboard.h>
#include <iostream>
#include <thread>
#include <chrono>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <math/vec3.hpp>
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


constexpr f32 NEAR_CLIP_DISTANCE = 0.1f;
constexpr f32 FAR_CLIP_DISTANCE = 1000.f;

namespace gl2 {
    math::vec3 intersect_z0_plane (math::vec3 ray_start, math::vec3 vector) {
        return intersect_plane(ray_start, vector, math::vec3(0.f), math::vec3(0.f, 0.f, 1.f));
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

        window = new Window(1280, 720);
        i32 err = window->startup();

        Keymap::instance();
        GraphicsApi::startup();
        Renderer::init();

        return err;
    }

    int Application::run() {
        running = true;

        LOG_DEBUG << "Graphics API: " << to_string(GraphicsApi::get_api_name());

        i32 w = window->get_width();
        i32 h = window->get_height();

        f32 window_ratio = f32(w) / f32(h);

        // glm::mat4 projection = glm::ortho(-(f32)w*.5f/h, (f32)w*.5f/h, -.5f, .5f);
        auto projection = math::projection(window_ratio * NEAR_CLIP_DISTANCE, NEAR_CLIP_DISTANCE, NEAR_CLIP_DISTANCE, FAR_CLIP_DISTANCE);

#ifdef GRAVITY
        Model model;
#endif

#ifdef CREATURES
        simulation model;
#endif

        Camera2D camera;

        Dispatcher<Mouse::ButtonPressEvent>::subscribe([&] (Mouse::ButtonPressEvent e) {
            if (e.button == Mouse::LEFT) {
                auto& m = Mouse::instance();
                // printf("==========================\n");
                
                math::vec3 position = math::vec3(camera.position.x, camera.position.y, camera.position.z);
                // printf("position = (%5.2f, %5.2f, %5.2f)\n", position.x, position.y, position.z);
                
                
                math::vec3 forward = camera.get_forward_vector_math();
                math::vec3 up = camera.get_up_vector_math();
                math::vec3 right = math::cross(forward, up);

                // f32 t = 2 * NEAR_CLIP_DISTANCE * ::std::tan(.5f * math::radians(45.f));
                // f32 clip_width = t * window_ratio;
                // f32 clip_height = t;

                f32 clip_width = NEAR_CLIP_DISTANCE * window_ratio;
                f32 clip_height = NEAR_CLIP_DISTANCE;

                // printf("NEAR_CLIP{ WIDTH = %5.2f; HEIGHT = %5.2f; }\n", clip_width, clip_height);

                math::vec2 mouse = math::vec2(
                    ( f32(m.x) / f32(w) - .5f),
                    (-f32(m.y) / f32(h) + .5f)
                );
                // printf("mouse = (%5.2f, %5.2f)\n", mouse.x, mouse.y);

                math::vec3 center = position + forward * NEAR_CLIP_DISTANCE;
                math::vec3 point = center + right*mouse.x * clip_width + up*mouse.y * clip_height;
                // printf("point = (%5.2f, %5.2f, %5.2f)\n", point.x, point.y, point.z);

                math::vec3 direction = point - position;

                math::vec3 intersection = intersect_z0_plane(position, direction);
                // printf("intersection = (%5.2f, %5.2f, %5.2f)\n", intersection.x, intersection.y, intersection.z);

#ifdef GRAVITY
                model.add_body(intersection.x, intersection.y);
#endif
#ifdef CREATURES
                model.add_creature(creature::make_random(math::vector2(intersection.x, intersection.y)));
#endif
            }
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
        

        // uncomment this call to draw in wireframe polygons.
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        while (running) {
            auto t_ = std::chrono::steady_clock::now();
            auto dt = std::chrono::duration_cast<std::chrono::microseconds>(t_ - t).count();
            // printf("dt = %ld μs; fps = %lf\n", dt, 1000000.0 / dt);
            t = t_;

            auto view = camera.get_view_matrix();
        
            body_shader.set_uniform_mat4f("u_view", view);
            body_shader.set_uniform_mat4f("u_model", glm::mat4(1.f));

            arrow_shader.set_uniform_mat4f("u_view", view);
            arrow_shader.set_uniform_mat4f("u_model", glm::mat4(1.f));

            Renderer::clear(math::color24::make(.8f));
            // Renderer::draw(va, ib, shdr);

            // Draw Ox
            {
                arrow_shader.set_uniform_mat4f("u_model", glm::translate(glm::scale(glm::mat4(1.f), glm::vec3(0.1f)), glm::vec3(.5f, 0.f, 0.f)));
                arrow_shader.set_uniform_3f("u_color", 0.f, 0.f, 1.f);
                arrow_shader.bind();
                Renderer::draw(va, ib, arrow_shader);
            }

            // Draw Oy
            {
                arrow_shader.set_uniform_mat4f("u_model", glm::translate(glm::rotate(glm::scale(glm::mat4(1.f), glm::vec3(0.1f)), glm::radians(90.f), glm::vec3(0.f, 0.f, 1.f)), glm::vec3(.5f, 0.f, 0.f)));
                arrow_shader.set_uniform_3f("u_color", 1.f, 0.f, 0.f);
                arrow_shader.bind();
                Renderer::draw(va, ib, arrow_shader);
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

