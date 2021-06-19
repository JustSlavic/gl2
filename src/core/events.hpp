#ifndef GL2_SRC_CORE_EVENTS_HPP
#define GL2_SRC_CORE_EVENTS_HPP

#include "event.hpp"
#include <math.hpp>


namespace core {


struct event_exit : public event {
    EVENT_DECLARE_CATEGORY(event::category::INPUT_EVENT);
    EVENT_DECLARE_TYPE(event::type::ACTION_EXIT);
};


struct event_camera_move : public event {
    EVENT_DECLARE_CATEGORY(event::category::INPUT_EVENT);
    EVENT_DECLARE_TYPE(event::type::ACTION_MOVE_CAMERA_LEFT);

    enum class direction : u8 { none, left, right, up, down };
    enum class type : u8 { start, stop };

    direction d = direction::none;
    type t = type::start;

    event_camera_move(direction direction_, type type_) : d(direction_), t(type_) {}
};


struct event_toggle_velocities : public event {
    EVENT_DECLARE_CATEGORY(event::category::ACTION_EVENT);
    EVENT_DECLARE_TYPE(event::type::ACTION_TOGGLE_VELOCITIES);
};


struct event_toggle_traces : public event {
    EVENT_DECLARE_CATEGORY(event::category::ACTION_EVENT);
    EVENT_DECLARE_TYPE(event::type::ACTION_TOGGLE_TRACES);
};


struct event_toggle_vector_field : public event {
    EVENT_DECLARE_CATEGORY(event::category::ACTION_EVENT);
    EVENT_DECLARE_TYPE(event::type::ACTION_TOGGLE_VECTOR_FIELD);
};


struct event_toggle_F2 : public event {
    EVENT_DECLARE_CATEGORY(event::category::ACTION_EVENT);
    EVENT_DECLARE_TYPE(event::type::ACTION_TOGGLE_F2);
};


} // core

#endif // GL2_SRC_CORE_EVENTS_HPP
