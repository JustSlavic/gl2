#include <application.h>
#include <iostream>
#include <thread>
#include <chrono>

#include <config.hpp>

#include <core/event_queue.hpp>

#include <math/vector3.hpp>
#include <graphics/graphics_api.h>
#include <graphics/shader.h>
#include <graphics/vertex_array.h>
#include <graphics/vertex_buffer.h>
#include <graphics/index_buffer.h>
#include <graphics/renderer.h>

#include <modeling_2d/camera.hpp>
#include <modeling_2d/model.hpp>

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


application::application() {
    bind<core::EventStop, application, &application::on_stop>(this);
}


application::~application() {
    window->shutdown();
    delete window;

    GraphicsApi::shutdown();
}


int application::initialize() {
    if (window) return 1;

    bool config_initialized = config::initialize("config.son");
    if (not config_initialized) {
        printf("Could not initialize config!\n");
        return 1;
    }

    auto& cfg = config::get_instance();
    printf("Window %s: %dx%d\n",
        cfg.name.c_str(), cfg.window.width, cfg.window.height);

    window = new Window(cfg.window.width, cfg.window.height, cfg.name.c_str());

    i32 err = window->startup();
    if (err) return err;

    GraphicsApi::startup();
    Renderer::init();

    layers.push_back(new core::LayerWorld());

    return 0;
}


int application::run() {
    running = true;

    auto& cfg = config::get_instance();
    math::color24 background_color{ cfg.window.default_color.r, cfg.window.default_color.g, cfg.window.default_color.b };

    auto t = std::chrono::steady_clock::now();
    std::this_thread::sleep_for(std::chrono::milliseconds(16));

    while (running) {
        auto t_ = std::chrono::steady_clock::now();
        auto dt = std::chrono::duration_cast<std::chrono::microseconds>(t_ - t).count();
        //printf("%llu: dt = %ld microseconds; fps = %lf\n", frame_counter++, dt, 1000000.0 / dt);
        t = t_;

        // 1. clear the frame
        Renderer::clear(background_color);

        // 2. draw new frame
        for (auto& layer : layers) {
            layer->draw();
        }

        // 3. Aquire events, swap buffers
        window->poll_events();
        window->swap_buffers();

        // 4. Frame is finished
        core::EventQueue::push_event<core::EventFrameFinished>(dt / 1'000'000.f);

        // 5. Handle events
        while (!core::EventQueue::empty()) {
            std::shared_ptr<core::IEvent> e = core::EventQueue::get_event();
            if (!e) continue;

            bool handled = handle(e.get());
            if (!handled) {
                for (auto& layer : layers) {
                    handled = layer->handle(e.get());
                    if (handled) break;
                }
            }

            core::EventQueue::pop_event();
        }
    }

    return 0;
}


bool application::on_stop(core::EventStop*) {
    printf("Received EventStop\n");
    running = false;

    return true;
}


} // gl2

