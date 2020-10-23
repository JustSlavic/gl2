#include <application.h>
#include <keymap.h>
#include <api/mouse.h>
#include <iostream>
#include <thread>
#include <chrono>


namespace gl2 {
    // Instantiating template BEFORE first usage
    template <>
    void Application::update<WindowMotionEvent>(WindowMotionEvent event) {
        LOG_INFO << "Received WindowMotionEvent(" << event.x << ", " << event.y << ")";
    }

    template <>
    void Application::update<EventMoveUp>(EventMoveUp) {
        LOG_INFO << "UP";
    }

    template <>
    void Application::update<EventMoveDown>(EventMoveDown) {
        LOG_INFO << "DOWN";
    }

    Application::Application() {
        Dispatcher<StopEvent>::subscribe(EVENT_CALLBACK(on_stop));
        Dispatcher<WindowMotionEvent>::subscribe(EVENT_CALLBACK(update));
        Dispatcher<EventMoveUp>::subscribe(EVENT_CALLBACK(update));
        Dispatcher<EventMoveDown>::subscribe(EVENT_CALLBACK(update));
    }

    Application::~Application() {
        window->shutdown();
        delete window;
    }

    int Application::initialize() {
        if (window) return 1;

        Keymap::instance();
        window = new Window(1280, 720, "gl2");
        return window->startup();
    }

    int Application::run() {
        running = true;

        while (running) {
            window->poll_events();
            window->swap_buffers();

            using namespace std::chrono_literals;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        return 0;
    }

    void Application::on_stop(StopEvent) {
        LOG_INFO << "Received StopEvent";
        running = false;
    }
}
