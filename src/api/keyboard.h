#pragma once

#include <es/event_system.h>


struct Keyboard : public IEmitter {
    enum class KeyState {
        RELEASED,
        PRESSED,
    };

    enum Key {
        ESC = 0,
        KEYBOARD_KEYS_N,
    };

    struct KeyPressEvent {
        Keyboard::Key key;

        KeyPressEvent(Key k) :key(k) {}
    };

    struct KeyReleaseEvent {
        Keyboard::Key key;

        KeyReleaseEvent(Key k) :key(k) {}
    };

    KeyState state[KEYBOARD_KEYS_N];

    static void press(Key);
    static void release(Key);
    static KeyState get_state(Key);
    static Keyboard &instance();

private:
    Keyboard();
};

