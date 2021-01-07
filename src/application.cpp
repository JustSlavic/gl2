#include <application.h>
#include <keymap.h> 
#include <api/mouse.h>
#include <api/keyboard.h>
#include <iostream>
#include <thread>
#include <chrono>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <fs/watcher.h>
#include <graphics/graphics_api.h>
#include <graphics/shader.h>
#include <graphics/vertex_array.h>
#include <graphics/vertex_buffer.h>
#include <graphics/index_buffer.h>
#include <graphics/renderer.h>

#include <modeling_2d/camera.hpp>
#include <modeling_2d/model.hpp>

#include <service/shader_library.hpp>


namespace gl2 {
    // Instantiating template BEFORE first usage
    Application::Application() {
        Dispatcher<EventStop>::subscribe(EVENT_CALLBACK(on_stop));
        Dispatcher<Keyboard::KeyPressEvent>::subscribe([](Keyboard::KeyPressEvent event) {
            LOG_DEBUG << "Keyboard{ " << to_string(event.key) << " };";
        });
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
        // service::shader::ShaderLibrary lib;
        // lib.initialize("shaders.data");
        // return 0;

        running = true;

        LOG_DEBUG << "Graphics API: " << to_string(GraphicsApi::get_api_name());

        i32 w = window->get_width();
        i32 h = window->get_height();
        // glm::mat4 projection = glm::ortho(-(f32)w*.5f/h, (f32)w*.5f/h, -.5f, .5f);
        auto projection = glm::perspective(glm::radians(45.0f), (f32)w / (f32)h, .1f, 1000.0f);

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                std::cout << projection[i][j] << " ";
            }
            std::cout << std::endl;
        }

        Model model;
        Camera2D camera;

        Dispatcher<Mouse::ButtonPressEvent>::subscribe([&] (Mouse::ButtonPressEvent e) {
            if (e.button == Mouse::LEFT) {
                auto& m = Mouse::instance();

                // auto screen_pos = glm::vec2(m.x, m.y) / glm::vec2(w, h) - .5f;
                // screen_pos *= glm::vec2((f32) w / h, -1.f);

                // auto unproj = glm::unProject(
                //     glm::vec3(m.x, h - 1 - m.y, 0.f),
                //     camera.get_view_matrix(),
                //     projection,
                //     glm::vec4(0.f, 0.f, w, h)
                //     );

                // LOG_DEBUG << "(" << unproj.x << ", " << unproj.y << ", " << unproj.z << ");";

                f32 x = ((f32) m.x / w - .5) * w / h + camera.position.x;
                f32 y = -((f32) m.y / h - .5f) + camera.position.y;

                model.add_body(x, y);
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
        model.arrow_shader = &arrow_shader;


        auto t = std::chrono::steady_clock::now();
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
        

        // uncomment this call to draw in wireframe polygons.
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        while (running) {
            auto t_ = std::chrono::steady_clock::now();
            auto dt = std::chrono::duration_cast<std::chrono::microseconds>(t_ - t).count();
            // LOG_DEBUG << "dt = " << dt << " μs; fps = " << (1000000.0 / dt);
            t = t_;

            auto view = camera.get_view_matrix();
        
            body_shader.set_uniform_mat4f("u_view", view);
            body_shader.set_uniform_mat4f("u_model", glm::mat4(1.f));

            arrow_shader.set_uniform_mat4f("u_view", view);
            arrow_shader.set_uniform_mat4f("u_model", glm::mat4(1.f));

            Renderer::clear(glm::vec3(.8f));
            // Renderer::draw(va, ib, shdr);

            model.draw_bodies();

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
