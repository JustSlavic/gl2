#ifndef GL2_SRC_CORE_INPUT_DEVICES_HPP
#define GL2_SRC_CORE_INPUT_DEVICES_HPP


#include <defines.hpp>
#include <stdio.h>


namespace core::input {


struct device {
    using id_t = uint32_t;

    enum class category : uint8_t {
        unknown,
        keyboard,
        mouse,
        gamepad,
        joystick,
        wheel,
    };

    uint32_t device_id;
    category m_category = category::unknown;

    device(category c, uint32_t id) : device_id(id), m_category(c) {}
    id_t get_id() const { return device_id; }

    category get_category() const { return m_category; }
    static category get_static_category() { return category::unknown; }
};


struct keyboard : public device {
    enum key {
        ESC = 0,
        _1, _2, _3, _4, _5, _6, _7, _8, _9, _0,
        F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
        A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
        LEFT, UP, RIGHT, DOWN,
        LALT, LCTRL, LSHIFT,
        RALT, RCTRL, RSHIFT,
        SPACE, ENTER, TAB, BACKQUOTE,
        BRACKET_OPEN, BRACKET_CLOSE,
        SEMICOLON, QUOTE, BACKSLASH, COMMA, DOT, SLASH,
        CAPSLOCK, SCROLLLOCK, NUMLOCK,
        INSERT, DELETE, HOME, END, PAGE_UP, PAGE_DOWN,
        NUM_1, NUM_2, NUM_3, NUM_4, NUM_5, NUM_6, NUM_7, NUM_8, NUM_9, NUM_0,
        NUM_DIV, NUM_MUL, NUM_MINUS, NUM_PLUS, NUM_ENTER,
        KEYBOARD_KEY_MAX,
    };

    enum key_state {
        RELEASED,
        PRESSED,
    };

    key_state state[KEYBOARD_KEY_MAX];

    keyboard(uint32_t id) : device(category::keyboard, id), state{} {}

    inline void press(key k) { state[k] = PRESSED; }
    inline void release(key k) { state[k] = RELEASED; }
    inline bool is_pressed(key k) { return state[k] == PRESSED; }
    inline bool is_released(key k) { return state[k] == RELEASED; }

    static category get_static_category() { return category::keyboard; }
};


struct mouse : public device {
    enum key {
        LEFT,
        MIDDLE,
        RIGHT,
        X1,
        X2,
        MOUSE_KEY_MAX,
    };

    enum key_state {
        RELEASED,
        PRESSED,
    };

    key_state state[MOUSE_KEY_MAX];
    i32 x = 0;
    i32 y = 0;

    mouse(uint32_t id) : device(category::mouse, id), state{} {}

    inline void press(key k) { state[k] = PRESSED; }
    inline void release(key k) { state[k] = RELEASED; }

    inline bool is_pressed(key k) { return state[k] == PRESSED; }
    inline bool is_released(key k) { return state[k] == RELEASED; }

    inline void scroll(i32 amount) {}
    inline void move(i32 new_x, i32 new_y) { x = new_x; y = new_y; }

    static category get_static_category() { return category::mouse; }
};


struct gamepad : public device {
    enum key {
        A,
        B,
        X,
        Y,
        LB,
        RB,
        START,
        BACK,
        DPAD_LEFT,
        DPAD_UP,
        DPAD_RIGHT,
        DPAD_DOWN,
        STICK_LEFT,
        STICK_RIGHT,
        GAMEPAD_KEY_MAX,
    };

    enum key_state {
        RELEASED,
        PRESSED,
    };

    enum axis {
        STICK_LEFT_X,
        STICK_LEFT_Y,
        STICK_RIGHT_X,
        STICK_RIGHT_Y,
        TRIGGER_LEFT,
        TRIGGER_RIGHT,
        GAMEPAD_AXIS_MAX,
    };

    key_state state[GAMEPAD_KEY_MAX];
    f32 axis_state[GAMEPAD_AXIS_MAX];

    gamepad(uint32_t id) : device(category::gamepad, id) {}

    inline void press(key k) { state[k] = PRESSED; }
    inline void release(key k) { state[k] = RELEASED; }

    inline bool is_pressed(key k) { return state[k] == PRESSED; }
    inline bool is_released(key k) { return state[k] == RELEASED; }

    inline void move_axis(axis a, f32 value) { axis_state[a] = value; }

    inline f32 get_axis(axis a) const { return axis_state[a]; }

    static category get_static_category() { return category::gamepad; }
};


} // core::input


#endif // GL2_SRC_CORE_INPUT_DEVICES_HPP
