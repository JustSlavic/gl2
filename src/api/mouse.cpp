#include "mouse.h"


Mouse::Mouse() :state{ButtonState::RELEASED} {}

Mouse &Mouse::instance() {
    static Mouse instance_;
    return instance_;
}

void Mouse::press(Button button) {
    Mouse& m = Mouse::instance();
    ASSERT(m.state[button] == ButtonState::RELEASED);
    m.state[button] = ButtonState::PRESSED;
    m.emit(ButtonPressEvent(button));
}

void Mouse::release(Button button) {
    Mouse& m = Mouse::instance();
    ASSERT(m.state[button] == ButtonState::PRESSED);
    m.state[button] = ButtonState::RELEASED;
    m.emit(ButtonReleaseEvent(button));
}

Mouse::ButtonState Mouse::get_state(Button button) {
    Mouse& m = Mouse::instance();
    return m.state[button];
}
