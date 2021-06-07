#include "mouse.h"
#include <core/event_queue.hpp>


Mouse::Mouse() :state{ButtonState::RELEASED} {}


Mouse::ButtonState Mouse::get_state(Button button) {
    Mouse& m = Mouse::instance();
    return m.state[button];
}


Mouse &Mouse::instance() {
    static Mouse instance_;
    return instance_;
}


void Mouse::press(Button button) {
    Mouse& m = Mouse::instance();
    // ASSERT(m.state[button] == ButtonState::RELEASED);
    m.state[button] = ButtonState::PRESSED;

    core::event_system::emit<ButtonPressEvent>(button);
}


void Mouse::release(Button button) {
    Mouse& m = Mouse::instance();
    ASSERT(m.state[button] == ButtonState::PRESSED);
    m.state[button] = ButtonState::RELEASED;

    core::event_system::emit<ButtonReleaseEvent>(button);
}


void Mouse::scroll(i32 scroll) {
    Mouse& m = Mouse::instance();

    core::event_system::emit<ScrollEvent>(scroll);
}


void Mouse::move(i32 x, i32 y) {
    Mouse& m = Mouse::instance();
    m.x = x;
    m.y = y;

    core::event_system::emit<MoveEvent>(x, y);
}
