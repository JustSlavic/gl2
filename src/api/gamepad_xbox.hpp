#ifndef GL2_SRC_API_GAMEPAD_XBOX_HPP
#define GL2_SRC_API_GAMEPAD_XBOX_HPP

#include <math/vector2.hpp>
#include <core/events.hpp>


struct Gamepad_XBox {
    enum class Button {
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
        GAMEPAD_XBOX_N,
    };

    enum class Axis {
        STICK_LEFT_X,
        STICK_LEFT_Y,
        STICK_RIGHT_X,
        STICK_RIGHT_Y,
        TRIGGER_LEFT,
        TRIGGER_RIGHT,
        GAMEPAD_AXIS_N,
    };

    enum class ButtonState {
        PRESSED,
        RELEASED,
    };

    struct Event_ButtonPressed : public core::IEvent {
        Button button;

        Event_ButtonPressed(Button button) : button(button) {}

        DECLARE_EVENT_TYPE(core::EventType::CONTROLLER_PRESS);
    };

    struct Event_ButtonReleased : public core::IEvent {
        Button button;

        Event_ButtonReleased(Button button) : button(button) {}

        DECLARE_EVENT_TYPE(core::EventType::CONTROLLER_RELEASE)
    };

    struct Event_AxisChanged : public core::IEvent {
        Axis axis;
        f32 old_value;
        f32 new_value;

        Event_AxisChanged(Axis a, f32 old_value, f32 new_value)
            : axis(a)
            , old_value(old_value)
            , new_value(new_value)
        {}

        DECLARE_EVENT_TYPE(core::EventType::CONTROLLER_AXIS_MOVE);
    };

    ButtonState state[static_cast<u64>(Button::GAMEPAD_XBOX_N)];
    f32 axis_state[static_cast<u64>(Axis::GAMEPAD_AXIS_N)];

    static void press(Button button);
    static void release(Button button);
    static ButtonState get_state(Button);
    static bool is_pressed(Button);
    static void set_axis(Axis axis, f32 value);

    static math::vector2 get_left_stick();
    static math::vector2 get_right_stick();
    static f32 get_left_trigger();
    static f32 get_right_trigger();

    static Gamepad_XBox& instance();

private:
    Gamepad_XBox();
    Gamepad_XBox(const Gamepad_XBox&) = delete;
    Gamepad_XBox(Gamepad_XBox&&) = delete;
};


#endif // GL2_SRC_API_GAMEPAD_XBOX_HPP
