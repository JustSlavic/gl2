#include "gamepad_xbox.hpp"
#include <core/event_queue.hpp>


#define TO_U64(x) (static_cast<u64>(x))

constexpr f32 deadzone = 0.05;


Gamepad_XBox::Gamepad_XBox() 
    : state{ ButtonState::RELEASED }
    , axis_state { 0 }
{}


Gamepad_XBox& Gamepad_XBox::instance() {
    static Gamepad_XBox instance_;
    return instance_;
}


void Gamepad_XBox::press(Button button) {
    Gamepad_XBox::instance().state[TO_U64(button)] = ButtonState::PRESSED;
    core::event_system::emit<Event_ButtonPressed>(button);
}


void Gamepad_XBox::release(Button button) {
    Gamepad_XBox::instance().state[TO_U64(button)] = ButtonState::RELEASED;
    core::event_system::emit<Event_ButtonReleased>(button);
}


Gamepad_XBox::ButtonState Gamepad_XBox::get_state(Button button) {
    return Gamepad_XBox::instance().state[TO_U64(button)];
}


bool Gamepad_XBox::is_pressed(Button button) {
    return Gamepad_XBox::instance().state[TO_U64(button)] == ButtonState::PRESSED;
}


void Gamepad_XBox::set_axis(Axis axis, f32 value) {
    auto& gpad = Gamepad_XBox::instance();
    f32 old_value = gpad.axis_state[TO_U64(axis)];

    if (math::abs(value) < deadzone) {
        value = 0.f;
    }
    
    if (math::equal(value, old_value)) {
        return;
    }

    gpad.axis_state[TO_U64(axis)] = value;
    core::event_system::emit<Event_AxisChanged>(axis, old_value, value);
}


math::vector2 Gamepad_XBox::get_left_stick() {
    return { Gamepad_XBox::instance().axis_state[TO_U64(Axis::STICK_LEFT_X)],
        Gamepad_XBox::instance().axis_state[TO_U64(Axis::STICK_LEFT_Y)] };
}


math::vector2 Gamepad_XBox::get_right_stick() {
    return { Gamepad_XBox::instance().axis_state[TO_U64(Axis::STICK_RIGHT_X)], 
        Gamepad_XBox::instance().axis_state[TO_U64(Axis::STICK_RIGHT_Y)] };
}


f32 Gamepad_XBox::get_left_trigger() {
    return Gamepad_XBox::instance().axis_state[TO_U64(Axis::TRIGGER_LEFT)];
}


f32 Gamepad_XBox::get_right_trigger() {
    return Gamepad_XBox::instance().axis_state[TO_U64(Axis::TRIGGER_RIGHT)];
}
