#include <SDL2/SDL.h>
#include <logging/logging.h>
#include <es/event_system.h>
#include "window.h"
#include "mouse.h"
#include "keyboard.h"


namespace gl2 {
    // @todo find means to remove opengl mentions from here, 
    //       amd move it to the another Api* class
    struct Window::Impl : public IEmitter {
        i32 width = 0;
        i32 height = 0;
        bool initialized = false;
        const char* title = nullptr;

        SDL_Window* window = nullptr;
        SDL_Surface* surface = nullptr;
        SDL_GLContext opengl_context = nullptr;

        Impl(i32 width, i32 height, const char* title) 
            : width(width)
            , height(height)
            , title(title)
        {}
        
        ~Impl() = default;

        i32 startup() {
            if (initialized) return 0;

            i32 err = SDL_Init(SDL_INIT_VIDEO);
            if (err < 0) {
                LOG_ERROR << "Could not initialize SDL2: " << SDL_GetError();
                return err;
            }

            window = SDL_CreateWindow(
                title,
                SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                width, height,
                SDL_WINDOW_SHOWN
            );

            surface = SDL_GetWindowSurface(window);
            opengl_context = SDL_GL_CreateContext(window);

            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
            initialized = true;
            return 0;
        }

        void shutdown() {
            if (not initialized) return;

            SDL_GL_DeleteContext(opengl_context);
            SDL_DestroyWindow(window);

            SDL_Quit();
            initialized = false;
        }

        void poll_events() {
            SDL_Event event;
            while (SDL_PollEvent(&event) != 0) {
                process_event(event);
            }
        }

        void swap_buffers() {
            SDL_GL_SwapWindow(window);
        }

    private:
        void process_event(const SDL_Event& event) {
            switch (event.type) {
                    case SDL_WINDOWEVENT:
                        if (event.window.event == SDL_WINDOWEVENT_MOVED) {
                            emit(WindowMotionEvent(event.window.data1, event.window.data2));
                        }
                        break;
                    case SDL_KEYDOWN:
                        process_keyboard_press_event(event.key);
                        break;
                    case SDL_KEYUP:
                        process_keyboard_release_event(event.key);
                        break;
                    case SDL_MOUSEMOTION:
                        process_mouse_motion_event(event.motion);
                        break;
                    case SDL_MOUSEBUTTONDOWN:
                        process_mouse_button_down_event(event.button);
                        break;
                    case SDL_MOUSEBUTTONUP:
                        process_mouse_button_up_event(event.button);                        
                        break;
                    case SDL_MOUSEWHEEL:
                        process_mouse_wheel_event(event);
                        break;
                    case SDL_QUIT: 
                        process_quit_event(event);
                        break;
                    default: LOG_WARNING << "Event of unknown type (" << event.type << ") emmited from SDL!";
                }
        }

        void process_mouse_button_down_event(const SDL_MouseButtonEvent& e) {
            switch (e.button) {
                case SDL_BUTTON_LEFT:
                    Mouse::press(Mouse::LEFT);
                    break;
                case SDL_BUTTON_MIDDLE:
                    Mouse::press(Mouse::MIDDLE);
                    break;
                case SDL_BUTTON_RIGHT:
                    Mouse::press(Mouse::RIGHT);
                    break;
                case SDL_BUTTON_X1:
                    Mouse::press(Mouse::X1);
                    break;
                case SDL_BUTTON_X2:
                    Mouse::press(Mouse::X2);
                    break;
                default: ASSERT(false);
            }
        }
        
        void process_mouse_button_up_event(const SDL_MouseButtonEvent& e) {
            switch (e.button) {
                case SDL_BUTTON_LEFT:
                    Mouse::release(Mouse::LEFT);
                    break;
                case SDL_BUTTON_MIDDLE:
                    Mouse::release(Mouse::MIDDLE);
                    break;
                case SDL_BUTTON_RIGHT:
                    Mouse::release(Mouse::RIGHT);
                    break;
                case SDL_BUTTON_X1:
                    Mouse::release(Mouse::X1);
                    break;
                case SDL_BUTTON_X2:
                    Mouse::release(Mouse::X2);
                    break;
                default: ASSERT(false);

            }
        }

        void process_mouse_motion_event(const SDL_MouseMotionEvent& e) {
            emit(MouseMotionEvent(e.x, e.y));            
        }

        void process_mouse_wheel_event(const SDL_Event& e) {
            // emit(MouseWheelEvent());
        }

        void process_quit_event(const SDL_Event& e) {
            emit(StopEvent());
        }

        void process_keyboard_press_event(const SDL_KeyboardEvent& e) {
            switch (e.keysym.sym) {
                case SDLK_ESCAPE:
                    Keyboard::press(Keyboard::ESC);
                    break;
                default: ASSERT(false);
            }
        }

        void process_keyboard_release_event(const SDL_KeyboardEvent& e) {
            switch (e.keysym.sym) {
                case SDLK_ESCAPE:
                    Keyboard::release(Keyboard::ESC);
                    break;
                default: ASSERT(false);
            }
        }
    };


    Window::Window(i32 width, i32 height)
        : Window(width, height, "GL2")
    {}

    Window::Window(i32 width, i32 height, const char* name)
        : impl(new Impl(width, height, name))
    {}

    Window::~Window() {
        delete impl;
    }

    i32 Window::startup() { return impl->startup(); }
    void Window::shutdown() { impl->shutdown(); }
    void Window::poll_events() { impl->poll_events(); }
    void Window::swap_buffers() { impl->swap_buffers(); }
}
