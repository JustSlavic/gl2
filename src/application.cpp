#include <application.h>
#include <keymap.h>
#include <api/mouse.h>
#include <api/keyboard.h>
#include <iostream>
#include <thread>
#include <chrono>

#include <fs/watcher.h>
#include <graphics/shader.h>


namespace gl2 {
    // Instantiating template BEFORE first usage
    Application::Application() {
        Dispatcher<StopEvent>::subscribe(EVENT_CALLBACK(on_stop));
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

        Keymap::instance();
        GraphicsApi::startup();

        window = new Window(1280, 720);
        return window->startup();
    }

    int Application::run() {
        running = true;

        LOG_DEBUG << "Graphics API: " << to_string(GraphicsApi::get_api_name());

        while (running) {
            window->poll_events();
            window->swap_buffers();

            emit<EventFrameFinished>();
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
        }

        return 0;
    }

    void Application::on_stop(StopEvent) {
        LOG_INFO << "Received StopEvent";
        running = false;
    }
}
