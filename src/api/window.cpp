#include "window.h"

#ifdef PLATFORM_WINDOWS
#include <SDL.h>
#endif

#ifdef PLATFORM_LINUX
#include <SDL2/SDL.h>
#endif

#include <logging/logging.h>
#include <es/event_system.h>
#include <core/event_queue.hpp>
#include <core/input.hpp>
#include <math.hpp>


namespace gl2 {
    constexpr f32 GL2_SDL_AXIS_MAX_VALUE = 32767.f;
    constexpr f32 GL2_SDL_AXIS_MIN_VALUE = -32768.f;

    // @todo find means to remove opengl mentions from here, 
    //       amd move it to the another Api* class
    struct Window::Impl : public IEmitter {
        SDL_Window* window = nullptr;
        SDL_Surface* surface = nullptr;
        SDL_GLContext opengl_context = nullptr;
        SDL_GameController* xbox_controller = nullptr;

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

            i32 err = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER);
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

            close_controller();

            SDL_Quit();
            initialized = false;
        }

        void close_controller() {
            if (xbox_controller != nullptr) {
                SDL_GameControllerClose(xbox_controller);
                xbox_controller = nullptr;
            }
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

                    case SDL_JOYAXISMOTION:          /**< Joystick axis motion */
                        // printf("SDL_JOYAXISMOTION\n");
                        break;
                    case SDL_JOYBALLMOTION:          /**< Joystick trackball motion */
                        printf("SDL_JOYBALLMOTION\n");
                        break;
                    case SDL_JOYHATMOTION:           /**< Joystick hat position change */
                        printf("SDL_JOYHATMOTION\n");
                        break;
                    case SDL_JOYBUTTONDOWN:          /**< Joystick button pressed */
                        printf("SDL_JOYBUTTONDOWN\n");
                        process_controller_button_press_event(event);
                        break;
                    case SDL_JOYBUTTONUP:            /**< Joystick button released */
                        printf("SDL_JOYBUTTONUP\n");
                        break;
                    case SDL_JOYDEVICEADDED:         /**< A new joystick has been inserted into the system */
                        printf("SDL_JOYDEVICEADDED\n");
                        break;
                    case SDL_JOYDEVICEREMOVED:       /**< An opened joystick has been removed */
                        printf("SDL_JOYDEVICEREMOVED\n");
                        break;

                    case SDL_CONTROLLERAXISMOTION: /**< Game controller axis motion */
                        process_controller_axis_event(event);
                        break;
                    case SDL_CONTROLLERBUTTONDOWN:          /**< Game controller button pressed */
                        process_controller_button_press_event(event);
                        break;
                    case SDL_CONTROLLERBUTTONUP:            /**< Game controller button released */
                        printf("SDL_CONTROLLERBUTTONUP\n");
                        break;
                    case SDL_CONTROLLERDEVICEADDED:         /**< A new Game controller has been inserted into the system */
                    case SDL_CONTROLLERDEVICEREMOVED:       /**< An opened Game controller has been removed */
                    case SDL_CONTROLLERDEVICEREMAPPED:      /**< The controller mapping was updated */
                        process_controller_device_event(event);
                        break;
                    case SDL_CONTROLLERTOUCHPADDOWN:        /**< Game controller touchpad was touched */
                        printf("SDL_CONTROLLERTOUCHPADDOWN\n");
                        break;
                    case SDL_CONTROLLERTOUCHPADMOTION:      /**< Game controller touchpad finger was moved */
                        printf("SDL_CONTROLLERTOUCHPADMOTION\n");
                        break;
                    case SDL_CONTROLLERTOUCHPADUP:          /**< Game controller touchpad finger was lifted */
                        printf("SDL_CONTROLLERTOUCHPADUP\n");
                        break;
                    case SDL_CONTROLLERSENSORUPDATE:        /**< Game controller sensor was updated */
                        printf("SDL_CONTROLLERSENSORUPDATE\n");
                        break;

                    case SDL_QUIT: 
                        process_quit_event(event);
                        break;
                    
                    case SDL_TEXTINPUT: break; // ignore this events
                    case SDL_TEXTEDITING: break; // ignore
                    default: printf("Event of unknown type (%d) emmited from SDL!", event.type);
                }
        }

        void process_controller_device_event(const SDL_Event& e) {
            switch (e.cdevice.type) {
            case SDL_CONTROLLERDEVICEADDED:
                printf("SDL_CONTROLLERDEVICEADDED(%d)\n", e.cdevice.which);
                if (xbox_controller != nullptr) {
                    // free controller
                }
                xbox_controller = SDL_GameControllerOpen(e.cdevice.which);
                break;
            case SDL_CONTROLLERDEVICEREMOVED:
                close_controller();
                break;
            case SDL_CONTROLLERDEVICEREMAPPED:
                printf("SDL_CONTROLLERDEVICEREMAPPED\n");
                break;
            }
        }

        void process_controller_axis_event(const SDL_Event& e) {
            switch (e.caxis.axis) {
            case SDL_CONTROLLER_AXIS_INVALID:
                printf("INVALID AXIS!\n");
                break;
            case SDL_CONTROLLER_AXIS_LEFTX:
                core::input::device_manager::gamepad_axis_move(
                    e.caxis.which,
                    core::input::gamepad::axis::STICK_LEFT_X,
                    math::clamp(e.caxis.value / GL2_SDL_AXIS_MAX_VALUE, -1.f, 1.f));
                break;
            case SDL_CONTROLLER_AXIS_LEFTY:
                core::input::device_manager::gamepad_axis_move(
                    e.caxis.which,
                    core::input::gamepad::axis::STICK_LEFT_Y,
                    math::clamp(e.caxis.value / GL2_SDL_AXIS_MAX_VALUE, -1.f, 1.f));
                break;
            case SDL_CONTROLLER_AXIS_RIGHTX:
                core::input::device_manager::gamepad_axis_move(
                    e.caxis.which,
                    core::input::gamepad::axis::STICK_RIGHT_X,
                    math::clamp(e.caxis.value / GL2_SDL_AXIS_MAX_VALUE, -1.f, 1.f));
                break;
            case SDL_CONTROLLER_AXIS_RIGHTY:
                core::input::device_manager::gamepad_axis_move(
                    e.caxis.which,
                    core::input::gamepad::axis::STICK_RIGHT_Y,
                    math::clamp(e.caxis.value / GL2_SDL_AXIS_MAX_VALUE, -1.f, 1.f));
                break;
            case SDL_CONTROLLER_AXIS_TRIGGERLEFT:
                core::input::device_manager::gamepad_axis_move(
                    e.caxis.which,
                    core::input::gamepad::axis::TRIGGER_LEFT,
                    math::clamp(e.caxis.value / GL2_SDL_AXIS_MAX_VALUE, -1.f, 1.f));
                break;
            case SDL_CONTROLLER_AXIS_TRIGGERRIGHT:
                core::input::device_manager::gamepad_axis_move(
                    e.caxis.which,
                    core::input::gamepad::axis::TRIGGER_RIGHT,
                    math::clamp(e.caxis.value / GL2_SDL_AXIS_MAX_VALUE, -1.f, 1.f));
                break;
            case SDL_CONTROLLER_AXIS_MAX:
                printf("MAX(%5.2f)\n", (float)e.caxis.value / 32767.f);
                break;
            }
        }

        void process_controller_button_press_event(const SDL_Event& e) {
            switch (e.cbutton.button) {
                case SDL_CONTROLLER_BUTTON_INVALID:

                    break;
                case SDL_CONTROLLER_BUTTON_A:
                    // Gamepad_XBox::press(Gamepad_XBox::Button::A);
                    break;
                case SDL_CONTROLLER_BUTTON_B:
                    // Gamepad_XBox::press(Gamepad_XBox::Button::B);
                    break;
                case SDL_CONTROLLER_BUTTON_X:
                    // Gamepad_XBox::press(Gamepad_XBox::Button::X);
                    break;
                case SDL_CONTROLLER_BUTTON_Y:
                    // Gamepad_XBox::press(Gamepad_XBox::Button::Y);
                    break;
                case SDL_CONTROLLER_BUTTON_BACK:
                    // Gamepad_XBox::press(Gamepad_XBox::Button::BACK);
                    break;
                case SDL_CONTROLLER_BUTTON_GUIDE:
                    // what is this button
                    break;
                case SDL_CONTROLLER_BUTTON_START:
                    // Gamepad_XBox::press(Gamepad_XBox::Button::START);
                    break;
                case SDL_CONTROLLER_BUTTON_LEFTSTICK:
                    // Gamepad_XBox::press(Gamepad_XBox::Button::STICK_LEFT);
                    break;
                case SDL_CONTROLLER_BUTTON_RIGHTSTICK:
                    // Gamepad_XBox::press(Gamepad_XBox::Button::STICK_RIGHT);
                    break;
                case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
                    // what is this button
                    break;
                case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
                    // what is this button
                    break;
                case SDL_CONTROLLER_BUTTON_DPAD_UP:
                    // Gamepad_XBox::press(Gamepad_XBox::Button::DPAD_UP);
                    break;
                case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
                    // Gamepad_XBox::press(Gamepad_XBox::Button::DPAD_DOWN);
                    break;
                case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
                    // Gamepad_XBox::press(Gamepad_XBox::Button::DPAD_LEFT);
                    break;
                case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
                    // Gamepad_XBox::press(Gamepad_XBox::Button::DPAD_RIGHT);
                    break;
                case SDL_CONTROLLER_BUTTON_MAX:
                    break;
            }
        }

        void process_mouse_button_event(const SDL_Event& e) {
            // #define PRESS_OR_RELEASE(BUTTON) if (e.type == SDL_MOUSEBUTTONUP) { Mouse::release(BUTTON); } else { Mouse::press(BUTTON); } void(0)
            switch (e.button.button) {
                case SDL_BUTTON_LEFT:
                    // PRESS_OR_RELEASE(Mouse::LEFT);
                    break;
                case SDL_BUTTON_MIDDLE:
                    // PRESS_OR_RELEASE(Mouse::MIDDLE);
                    break;
                case SDL_BUTTON_RIGHT:
                    // PRESS_OR_RELEASE(Mouse::RIGHT);
                    break;
                case SDL_BUTTON_X1:
                    // PRESS_OR_RELEASE(Mouse::X1);
                    break;
                case SDL_BUTTON_X2:
                    // PRESS_OR_RELEASE(Mouse::X2);
                    break;

                default: ASSERT(false);
            }
            // #undef PRESS_OR_RELEASE
        }

        void process_mouse_motion_event(const SDL_MouseMotionEvent& e) {
            // Mouse::move(e.x, e.y);
        }

        void process_mouse_wheel_event(const SDL_Event& e) {
            // Mouse::scroll(e.wheel.y);
        }

        void process_quit_event(const SDL_Event& e) {
            emit(EventStop()); // @Todo Emit EventWindowClose instead
            core::EventQueue::push_event<core::EventStop>();
        }

        void process_keyboard_event(const SDL_Event& e) {
#define PRESS_OR_RELEASE(KEY) \
    if (e.type == SDL_KEYUP) { \
        core::input::device_manager::keyboard_press(KEY); \
    } else { \
        core::input::device_manager::keyboard_release(KEY); \
    } void(0)
             switch (e.key.keysym.scancode) {
                 case SDL_SCANCODE_ESCAPE: PRESS_OR_RELEASE(core::input::keyboard::ESC); break;
                 case SDL_SCANCODE_SPACE:  PRESS_OR_RELEASE(core::input::keyboard::SPACE); break;

                 case SDL_SCANCODE_LALT:   PRESS_OR_RELEASE(core::input::keyboard::LALT); break;
                 case SDL_SCANCODE_LCTRL:  PRESS_OR_RELEASE(core::input::keyboard::LCTRL); break;
                 case SDL_SCANCODE_LSHIFT: PRESS_OR_RELEASE(core::input::keyboard::LSHIFT); break;

                 case SDL_SCANCODE_RALT:   PRESS_OR_RELEASE(core::input::keyboard::RALT); break;
                 case SDL_SCANCODE_RCTRL:  PRESS_OR_RELEASE(core::input::keyboard::RCTRL); break;
                 case SDL_SCANCODE_RSHIFT: PRESS_OR_RELEASE(core::input::keyboard::RSHIFT); break;

                 case SDL_SCANCODE_GRAVE:  PRESS_OR_RELEASE(core::input::keyboard::BACKQUOTE); break;
                 case SDL_SCANCODE_TAB:    PRESS_OR_RELEASE(core::input::keyboard::TAB); break;
                
                 case SDL_SCANCODE_CAPSLOCK:     PRESS_OR_RELEASE(core::input::keyboard::CAPSLOCK); break;
                 case SDL_SCANCODE_SCROLLLOCK:   PRESS_OR_RELEASE(core::input::keyboard::SCROLLLOCK); break;
                 case SDL_SCANCODE_NUMLOCKCLEAR: PRESS_OR_RELEASE(core::input::keyboard::NUMLOCK); break;

                 case SDL_SCANCODE_0: PRESS_OR_RELEASE(core::input::keyboard::_0); break;
                 case SDL_SCANCODE_1: PRESS_OR_RELEASE(core::input::keyboard::_1); break;
                 case SDL_SCANCODE_2: PRESS_OR_RELEASE(core::input::keyboard::_2); break;
                 case SDL_SCANCODE_3: PRESS_OR_RELEASE(core::input::keyboard::_3); break;
                 case SDL_SCANCODE_4: PRESS_OR_RELEASE(core::input::keyboard::_4); break;
                 case SDL_SCANCODE_5: PRESS_OR_RELEASE(core::input::keyboard::_5); break;
                 case SDL_SCANCODE_6: PRESS_OR_RELEASE(core::input::keyboard::_6); break;
                 case SDL_SCANCODE_7: PRESS_OR_RELEASE(core::input::keyboard::_7); break;
                 case SDL_SCANCODE_8: PRESS_OR_RELEASE(core::input::keyboard::_8); break;
                 case SDL_SCANCODE_9: PRESS_OR_RELEASE(core::input::keyboard::_9); break;

                 case SDL_SCANCODE_F1: PRESS_OR_RELEASE(core::input::keyboard::F1); break;
                 case SDL_SCANCODE_F2: PRESS_OR_RELEASE(core::input::keyboard::F2); break;
                 case SDL_SCANCODE_F3: PRESS_OR_RELEASE(core::input::keyboard::F3); break;
                 case SDL_SCANCODE_F4: PRESS_OR_RELEASE(core::input::keyboard::F4); break;
                 case SDL_SCANCODE_F5: PRESS_OR_RELEASE(core::input::keyboard::F5); break;
                 case SDL_SCANCODE_F6: PRESS_OR_RELEASE(core::input::keyboard::F6); break;
                 case SDL_SCANCODE_F7: PRESS_OR_RELEASE(core::input::keyboard::F7); break;
                 case SDL_SCANCODE_F8: PRESS_OR_RELEASE(core::input::keyboard::F8); break;
                 case SDL_SCANCODE_F9: PRESS_OR_RELEASE(core::input::keyboard::F9); break;
                 case SDL_SCANCODE_F10: PRESS_OR_RELEASE(core::input::keyboard::F10); break;
                 case SDL_SCANCODE_F11: PRESS_OR_RELEASE(core::input::keyboard::F11); break;
                 case SDL_SCANCODE_F12: PRESS_OR_RELEASE(core::input::keyboard::F12); break;

                 case SDL_SCANCODE_A: PRESS_OR_RELEASE(core::input::keyboard::A); break;
                 case SDL_SCANCODE_B: PRESS_OR_RELEASE(core::input::keyboard::B); break;
                 case SDL_SCANCODE_C: PRESS_OR_RELEASE(core::input::keyboard::C); break;
                 case SDL_SCANCODE_D: PRESS_OR_RELEASE(core::input::keyboard::D); break;
                 case SDL_SCANCODE_E: PRESS_OR_RELEASE(core::input::keyboard::E); break;
                 case SDL_SCANCODE_F: PRESS_OR_RELEASE(core::input::keyboard::F); break;
                 case SDL_SCANCODE_G: PRESS_OR_RELEASE(core::input::keyboard::G); break;
                 case SDL_SCANCODE_H: PRESS_OR_RELEASE(core::input::keyboard::H); break;
                 case SDL_SCANCODE_I: PRESS_OR_RELEASE(core::input::keyboard::I); break;
                 case SDL_SCANCODE_J: PRESS_OR_RELEASE(core::input::keyboard::J); break;
                 case SDL_SCANCODE_K: PRESS_OR_RELEASE(core::input::keyboard::K); break;
                 case SDL_SCANCODE_L: PRESS_OR_RELEASE(core::input::keyboard::L); break;
                 case SDL_SCANCODE_M: PRESS_OR_RELEASE(core::input::keyboard::M); break;
                 case SDL_SCANCODE_N: PRESS_OR_RELEASE(core::input::keyboard::N); break;
                 case SDL_SCANCODE_O: PRESS_OR_RELEASE(core::input::keyboard::O); break;
                 case SDL_SCANCODE_P: PRESS_OR_RELEASE(core::input::keyboard::P); break;
                 case SDL_SCANCODE_Q: PRESS_OR_RELEASE(core::input::keyboard::Q); break;
                 case SDL_SCANCODE_R: PRESS_OR_RELEASE(core::input::keyboard::R); break;
                 case SDL_SCANCODE_S: PRESS_OR_RELEASE(core::input::keyboard::S); break;
                 case SDL_SCANCODE_T: PRESS_OR_RELEASE(core::input::keyboard::T); break;
                 case SDL_SCANCODE_U: PRESS_OR_RELEASE(core::input::keyboard::U); break;
                 case SDL_SCANCODE_V: PRESS_OR_RELEASE(core::input::keyboard::V); break;
                 case SDL_SCANCODE_W: PRESS_OR_RELEASE(core::input::keyboard::W); break;
                 case SDL_SCANCODE_X: PRESS_OR_RELEASE(core::input::keyboard::X); break;
                 case SDL_SCANCODE_Y: PRESS_OR_RELEASE(core::input::keyboard::Y); break;
                 case SDL_SCANCODE_Z: PRESS_OR_RELEASE(core::input::keyboard::Z); break;

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
