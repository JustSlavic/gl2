#ifndef GL2_SRC_CORE_EVENTS_HPP
#define GL2_SRC_CORE_EVENTS_HPP

#include "event.hpp"
#include <math.hpp>


namespace core {


void initialize_core_events();
void terminate_core_events();


struct event_exit : public event {
    EVENT_DECLARE(event::category::INPUT_EVENT);
};


struct event_camera_move : public event {
    EVENT_DECLARE(event::category::INPUT_EVENT);

    enum class direction : u8 { none, left, right, up, down };
    enum class type : u8 { start, stop };

    direction d = direction::none;
    type t = type::start;

    event_camera_move(direction direction_, type type_) : d(direction_), t(type_) {}
};


struct event_toggle_velocities : public event {
    EVENT_DECLARE(event::category::ACTION_EVENT);
};


struct event_toggle_traces : public event {
    EVENT_DECLARE(event::category::ACTION_EVENT);
};


struct event_toggle_vector_field : public event {
    EVENT_DECLARE(event::category::ACTION_EVENT);
};


struct event_toggle_F2 : public event {
    EVENT_DECLARE(event::category::ACTION_EVENT);
};


} // core

#endif // GL2_SRC_CORE_EVENTS_HPP
