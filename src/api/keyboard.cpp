#include "keyboard.h"


Keyboard::Keyboard() :state{KeyState::RELEASED} {}

Keyboard &Keyboard::instance() {
    static Keyboard instance_;
    return instance_;
}

void Keyboard::press(Key k) {
    auto& keyboard = Keyboard::instance();
    // ASSERT(keyboard.state[k] == KeyState::RELEASED);
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

const char *to_string(Keyboard::Key k) {
    switch (k) {
        case Keyboard::ESC: return "ESC";

        case Keyboard::_1: return "_1";
        case Keyboard::_2: return "_2";
        case Keyboard::_3: return "_3";
        case Keyboard::_4: return "_4";
        case Keyboard::_5: return "_5";
        case Keyboard::_6: return "_6";
        case Keyboard::_7: return "_7";
        case Keyboard::_8: return "_8";
        case Keyboard::_9: return "_9";
        case Keyboard::_0: return "_0";

        case Keyboard::F1: return "F1";
        case Keyboard::F2: return "F2";
        case Keyboard::F3: return "F3";
        case Keyboard::F4: return "F4";
        case Keyboard::F5: return "F5";
        case Keyboard::F6: return "F6";
        case Keyboard::F7: return "F7";
        case Keyboard::F8: return "F8";
        case Keyboard::F9: return "F9";
        case Keyboard::F0: return "F0";

        case Keyboard::A: return "A";
        case Keyboard::B: return "B";
        case Keyboard::C: return "C";
        case Keyboard::D: return "D";
        case Keyboard::E: return "E";
        case Keyboard::F: return "F";
        case Keyboard::G: return "G";
        case Keyboard::H: return "H";
        case Keyboard::I: return "I";
        case Keyboard::J: return "J";
        case Keyboard::K: return "K";
        case Keyboard::L: return "L";
        case Keyboard::M: return "M";
        case Keyboard::N: return "N";
        case Keyboard::O: return "O";
        case Keyboard::P: return "P";
        case Keyboard::Q: return "Q";
        case Keyboard::R: return "R";
        case Keyboard::S: return "S";
        case Keyboard::T: return "T";
        case Keyboard::U: return "U";
        case Keyboard::V: return "V";
        case Keyboard::W: return "W";
        case Keyboard::X: return "X";
        case Keyboard::Y: return "Y";
        case Keyboard::Z: return "Z";

        case Keyboard::NUM_1: return "NUM_1";
        case Keyboard::NUM_2: return "NUM_2";
        case Keyboard::NUM_3: return "NUM_3";
        case Keyboard::NUM_4: return "NUM_4";
        case Keyboard::NUM_5: return "NUM_5";
        case Keyboard::NUM_6: return "NUM_6";
        case Keyboard::NUM_7: return "NUM_7";
        case Keyboard::NUM_8: return "NUM_8";
        case Keyboard::NUM_9: return "NUM_9";
        case Keyboard::NUM_0: return "NUM_0";

        case Keyboard::LALT:   return "LALT";
        case Keyboard::LCTRL:  return "LCTRL";
        case Keyboard::LSHIFT: return "LSHIFT";

        case Keyboard::RALT:   return "RALT";
        case Keyboard::RCTRL:  return "RCTRL";
        case Keyboard::RSHIFT: return "RSHIFT";

        case Keyboard::SPACE:     return "SPACE";
        case Keyboard::ENTER:     return "ENTER";
        case Keyboard::TAB:       return "TAB";
        case Keyboard::BACKQUOTE: return "BACKQUOTE";

        case Keyboard::CAPSLOCK:   return "CAPSLOCK";
        case Keyboard::SCROLLLOCK: return "SCROLLLOCK";
        case Keyboard::NUMLOCK:    return "NUMLOCK";

        default: ASSERT(false);
    }

    return nullptr;
}


