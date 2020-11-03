#pragma once

#include <es/event_system.h>


struct Keyboard : public IEmitter {
    enum class KeyState {
        RELEASED,
        PRESSED,
    };

    enum Key {
        ESC = 0,
        _1, _2, _3, _4, _5, _6, _7, _8, _9, _0,
        F1, F2, F3, F4, F5, F6, F7, F8, F9, F0,
        A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
        NUM_1, NUM_2, NUM_3, NUM_4, NUM_5, NUM_6, NUM_7, NUM_8, NUM_9, NUM_0,
        LALT, LCTRL, LSHIFT,
        RALT, RCTRL, RSHIFT,
        SPACE, ENTER, TAB, BACKQUOTE,
        CAPSLOCK, SCROLLLOCK, NUMLOCK,
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


const char *to_string(Keyboard::Key k);
