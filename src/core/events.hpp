#ifndef GL2_SRC_CORE_EVENTS_HPP
#define GL2_SRC_CORE_EVENTS_HPP

#include "event.hpp"


namespace core {


struct event_exit : public event {
    EVENT_DECLARE_CATEGORY(event::category::INPUT_EVENT);
    EVENT_DECLARE_TYPE(event::type::ACTION_EXIT);
};


} // core

#endif // GL2_SRC_CORE_EVENTS_HPP
