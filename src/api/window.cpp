#include "window.h"

#ifdef PLATFORM_WINDOWS
#include <SDL.h>
#endif

#ifdef PLATFORM_LINUX
#include <SDL2/SDL.h>
#endif

#include <logging/logging.h>
#include <es/event_system.h>
#include "mouse.h"
#include "keyboard.h"


namespace gl2 {
    // @todo find means to remove opengl mentions from here, 
    //       amd move it to the another Api* class
    struct Window::Impl : public IEmitter {
        SDL_Window* window = nullptr;
        SDL_Surface* surface = nullptr;
        SDL_GLContext opengl_context = nullptr;

        const char* title = nullptr;
        i32 width = 0;
        i32 height = 0;
        bool initialized = false;

        Impl(i32 width, i32 height, const char* title) 
            : title(title)
            , width(width)
            , height(height)
        {}
        
        ~Impl() = default;

        i32 get_width() { return width; }
        i32 get_height() { return height; }

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
                SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL
            );

            surface = SDL_GetWindowSurface(window);
            opengl_context = SDL_GL_CreateContext(window);

            if (opengl_context == nullptr) {
                const char* error = SDL_GetError();
                printf("Counld not create OpenGL context: %s\n", error);
                return 2;
            }

            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

            SDL_version compiled;
            SDL_VERSION(&compiled);
            LOG_INFO << "Compiled against SDL v." << 
                (int)compiled.major << "." << (int)compiled.minor << "." << (int)compiled.patch;

            SDL_version linked;
            SDL_GetVersion(&linked);
            LOG_INFO << "Linked against SDL v." <<
                (int)linked.major << "." << (int)linked.minor << "." << (int)linked.patch;

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
                    case SDL_KEYUP:
                        process_keyboard_event(event);
                        break;
                    case SDL_MOUSEMOTION:
                        process_mouse_motion_event(event.motion);
                        break;
                    case SDL_MOUSEBUTTONDOWN:
                    case SDL_MOUSEBUTTONUP:
                        process_mouse_button_event(event);
                        break;
                    case SDL_MOUSEWHEEL:
                        process_mouse_wheel_event(event);
                        break;
                    case SDL_QUIT: 
                        process_quit_event(event);
                        break;
                    
                    case SDL_TEXTINPUT: break; // ignore this events
                    case SDL_TEXTEDITING: break; // ignore
                    default: LOG_WARNING << "Event of unknown type (" << event.type << ") emmited from SDL!";
                }
        }

        void process_mouse_button_event(const SDL_Event& e) {
            #define PRESS_OR_RELEASE(BUTTON) if (e.type == SDL_MOUSEBUTTONUP) { Mouse::release(BUTTON); } else { Mouse::press(BUTTON); } void(0)
            switch (e.button.button) {
                case SDL_BUTTON_LEFT:
                    PRESS_OR_RELEASE(Mouse::LEFT);
                    break;
                case SDL_BUTTON_MIDDLE:
                    PRESS_OR_RELEASE(Mouse::MIDDLE);
                    break;
                case SDL_BUTTON_RIGHT:
                    PRESS_OR_RELEASE(Mouse::RIGHT);
                    break;
                case SDL_BUTTON_X1:
                    PRESS_OR_RELEASE(Mouse::X1);
                    break;
                case SDL_BUTTON_X2:
                    PRESS_OR_RELEASE(Mouse::X2);
                    break;

                default: ASSERT(false);
            }
            #undef PRESS_OR_RELEASE
        }

        void process_mouse_motion_event(const SDL_MouseMotionEvent& e) {
            Mouse::move(e.x, e.y);
        }

        void process_mouse_wheel_event(const SDL_Event& e) {
            Mouse::scroll(e.wheel.y);
        }

        void process_quit_event(const SDL_Event& e) {
            emit(EventStop()); // @Todo Emit EventWindowClose instead
        }

        void process_keyboard_event(const SDL_Event& e) {
            #define PRESS_OR_RELEASE(KEY) if (e.type == SDL_KEYUP) { Keyboard::release(KEY); } else { Keyboard::press(KEY); } void(0)
            switch (e.key.keysym.scancode) {
                case SDL_SCANCODE_ESCAPE: PRESS_OR_RELEASE(Keyboard::ESC); break;
                case SDL_SCANCODE_SPACE:  PRESS_OR_RELEASE(Keyboard::SPACE); break;

                case SDL_SCANCODE_LALT:   PRESS_OR_RELEASE(Keyboard::LALT); break;
                case SDL_SCANCODE_LCTRL:  PRESS_OR_RELEASE(Keyboard::LCTRL); break;
                case SDL_SCANCODE_LSHIFT: PRESS_OR_RELEASE(Keyboard::LSHIFT); break;

                case SDL_SCANCODE_RALT:   PRESS_OR_RELEASE(Keyboard::RALT); break;
                case SDL_SCANCODE_RCTRL:  PRESS_OR_RELEASE(Keyboard::RCTRL); break;
                case SDL_SCANCODE_RSHIFT: PRESS_OR_RELEASE(Keyboard::RSHIFT); break;

                case SDL_SCANCODE_GRAVE:  PRESS_OR_RELEASE(Keyboard::BACKQUOTE); break;
                case SDL_SCANCODE_TAB:    PRESS_OR_RELEASE(Keyboard::TAB); break;
                
                case SDL_SCANCODE_CAPSLOCK:     PRESS_OR_RELEASE(Keyboard::CAPSLOCK); break;
                case SDL_SCANCODE_SCROLLLOCK:   PRESS_OR_RELEASE(Keyboard::SCROLLLOCK); break;
                case SDL_SCANCODE_NUMLOCKCLEAR: PRESS_OR_RELEASE(Keyboard::NUMLOCK); break;

                case SDL_SCANCODE_0: PRESS_OR_RELEASE(Keyboard::_0); break;
                case SDL_SCANCODE_1: PRESS_OR_RELEASE(Keyboard::_1); break;
                case SDL_SCANCODE_2: PRESS_OR_RELEASE(Keyboard::_2); break;
                case SDL_SCANCODE_3: PRESS_OR_RELEASE(Keyboard::_3); break;
                case SDL_SCANCODE_4: PRESS_OR_RELEASE(Keyboard::_4); break;
                case SDL_SCANCODE_5: PRESS_OR_RELEASE(Keyboard::_5); break;
                case SDL_SCANCODE_6: PRESS_OR_RELEASE(Keyboard::_6); break;
                case SDL_SCANCODE_7: PRESS_OR_RELEASE(Keyboard::_7); break;
                case SDL_SCANCODE_8: PRESS_OR_RELEASE(Keyboard::_8); break;
                case SDL_SCANCODE_9: PRESS_OR_RELEASE(Keyboard::_9); break;

                case SDL_SCANCODE_F1: PRESS_OR_RELEASE(Keyboard::F1); break;
                case SDL_SCANCODE_F2: PRESS_OR_RELEASE(Keyboard::F2); break;
                case SDL_SCANCODE_F3: PRESS_OR_RELEASE(Keyboard::F3); break;
                case SDL_SCANCODE_F4: PRESS_OR_RELEASE(Keyboard::F4); break;
                case SDL_SCANCODE_F5: PRESS_OR_RELEASE(Keyboard::F5); break;
                case SDL_SCANCODE_F6: PRESS_OR_RELEASE(Keyboard::F6); break;
                case SDL_SCANCODE_F7: PRESS_OR_RELEASE(Keyboard::F7); break;
                case SDL_SCANCODE_F8: PRESS_OR_RELEASE(Keyboard::F8); break;
                case SDL_SCANCODE_F9: PRESS_OR_RELEASE(Keyboard::F9); break;
                case SDL_SCANCODE_F10: PRESS_OR_RELEASE(Keyboard::F10); break;
                case SDL_SCANCODE_F11: PRESS_OR_RELEASE(Keyboard::F11); break;
                case SDL_SCANCODE_F12: PRESS_OR_RELEASE(Keyboard::F12); break;

                case SDL_SCANCODE_A: PRESS_OR_RELEASE(Keyboard::A); break;
                case SDL_SCANCODE_B: PRESS_OR_RELEASE(Keyboard::B); break;
                case SDL_SCANCODE_C: PRESS_OR_RELEASE(Keyboard::C); break;
                case SDL_SCANCODE_D: PRESS_OR_RELEASE(Keyboard::D); break;
                case SDL_SCANCODE_E: PRESS_OR_RELEASE(Keyboard::E); break;
                case SDL_SCANCODE_F: PRESS_OR_RELEASE(Keyboard::F); break;
                case SDL_SCANCODE_G: PRESS_OR_RELEASE(Keyboard::G); break;
                case SDL_SCANCODE_H: PRESS_OR_RELEASE(Keyboard::H); break;
                case SDL_SCANCODE_I: PRESS_OR_RELEASE(Keyboard::I); break;
                case SDL_SCANCODE_J: PRESS_OR_RELEASE(Keyboard::J); break;
                case SDL_SCANCODE_K: PRESS_OR_RELEASE(Keyboard::K); break;
                case SDL_SCANCODE_L: PRESS_OR_RELEASE(Keyboard::L); break;
                case SDL_SCANCODE_M: PRESS_OR_RELEASE(Keyboard::M); break;
                case SDL_SCANCODE_N: PRESS_OR_RELEASE(Keyboard::N); break;
                case SDL_SCANCODE_O: PRESS_OR_RELEASE(Keyboard::O); break;
                case SDL_SCANCODE_P: PRESS_OR_RELEASE(Keyboard::P); break;
                case SDL_SCANCODE_Q: PRESS_OR_RELEASE(Keyboard::Q); break;
                case SDL_SCANCODE_R: PRESS_OR_RELEASE(Keyboard::R); break;
                case SDL_SCANCODE_S: PRESS_OR_RELEASE(Keyboard::S); break;
                case SDL_SCANCODE_T: PRESS_OR_RELEASE(Keyboard::T); break;
                case SDL_SCANCODE_U: PRESS_OR_RELEASE(Keyboard::U); break;
                case SDL_SCANCODE_V: PRESS_OR_RELEASE(Keyboard::V); break;
                case SDL_SCANCODE_W: PRESS_OR_RELEASE(Keyboard::W); break;
                case SDL_SCANCODE_X: PRESS_OR_RELEASE(Keyboard::X); break;
                case SDL_SCANCODE_Y: PRESS_OR_RELEASE(Keyboard::Y); break;
                case SDL_SCANCODE_Z: PRESS_OR_RELEASE(Keyboard::Z); break;

                default: LOG_WARNING << "Unregistered key pressed: scancode(" << e.key.keysym.scancode <<") keycode(" << e.key.keysym.sym << ")";
            }
            #undef PRESS_OR_RELEASE
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
    i32 Window::get_width() { return impl->get_width(); }
    i32 Window::get_height() { return impl->get_height(); }
    void Window::poll_events() { impl->poll_events(); }
    void Window::swap_buffers() { impl->swap_buffers(); }
}
