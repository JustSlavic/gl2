#ifndef API_MOUSE_H
#define API_MOUSE_H

#include <es/event_system.h>


struct MouseButtonDownEvent;
struct MouseButtonUpEvent;

struct Mouse : public IEmitter {
    enum Button {
        LEFT,
        MIDDLE,
        RIGHT,
        X1,
        X2,
        MOUSE_BUTTONS_N,
    };

    enum class ButtonState {
        RELEASED,
        PRESSED,
    };

    ButtonState state[MOUSE_BUTTONS_N];

    static void press(Button button);
    static void release(Button button);
    static Mouse &instance();

private:
    Mouse();
};

inline const char *to_string(Mouse::Button b) {
    switch (b) {
        case Mouse::LEFT: return "LEFT";
        case Mouse::MIDDLE: return "MIDDLE";
        case Mouse::RIGHT: return "RIGHT";
        case Mouse::X1: return "X1";
        case Mouse::X2: return "X2";
        default: return "ERROR";
    }
}

struct MouseButtonDownEvent {
    Mouse::Button button;

    MouseButtonDownEvent(Mouse::Button button) :button(button) {}
};

struct MouseButtonUpEvent {
    Mouse::Button button;

    MouseButtonUpEvent(Mouse::Button button) :button(button) {}
};

#endif // API_MOUSE_H
