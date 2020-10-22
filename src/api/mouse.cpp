#include "mouse.h"


Mouse::Mouse() {}

Mouse &Mouse::instance() {
    static Mouse instance_;
    return instance_;
}

void Mouse::press(Button button) {
    Mouse& m = Mouse::instance();
    m.state[button] = ButtonState::PRESSED;
    m.emit(MouseButtonDownEvent(button));
}

void Mouse::release(Button button) {
    Mouse& m = Mouse::instance();
    m.state[button] = ButtonState::RELEASED;
    m.emit(MouseButtonUpEvent(button));
}

