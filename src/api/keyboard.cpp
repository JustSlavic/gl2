#include "keyboard.h"


Keyboard::Keyboard() :state{KeyState::RELEASED} {}

Keyboard &Keyboard::instance() {
    static Keyboard instance_;
    return instance_;
}

void Keyboard::press(Key k) {
    auto& keyboard = Keyboard::instance();
    ASSERT(keyboard.state[k] == KeyState::RELEASED);
    keyboard.state[k] = KeyState::PRESSED;
    keyboard.emit(KeyPressEvent(k));
}

void Keyboard::release(Key k) {
    auto& keyboard = Keyboard::instance();
    ASSERT(keyboard.state[k] == KeyState::PRESSED);
    keyboard.state[k] = KeyState::RELEASED;
    keyboard.emit(KeyReleaseEvent(k));
}

Keyboard::KeyState Keyboard::get_state(Key k) {
    auto& keyboard = Keyboard::instance();
    return keyboard.state[k];
} 
