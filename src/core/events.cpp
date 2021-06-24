#include "events.hpp"


namespace core {

event::id_t event_exit::id = 0;
event::id_t event_camera_move::id = 0;
event::id_t event_toggle_velocities::id = 0;
event::id_t event_toggle_traces::id = 0;
event::id_t event_toggle_vector_field::id = 0;
event::id_t event_toggle_F2::id = 0;

void initialize_core_events() {
    event::register_event<event_exit>();
    event::register_event<event_camera_move>();
    event::register_event<event_toggle_velocities>();
    event::register_event<event_toggle_traces>();
    event::register_event<event_toggle_vector_field>();
    event::register_event<event_toggle_F2>();
}


void terminate_core_events() {
}


} // core
