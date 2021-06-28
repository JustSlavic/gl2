#ifndef GL2_SRC_CORE_EVENT_HPP
#define GL2_SRC_CORE_EVENT_HPP

#include <defines.hpp>


namespace core {


struct event {
    using id_t = u32;

    enum class category {
        FRAME_EVENT,
        INPUT_EVENT,
        ACTION_EVENT,
    };

    bool handled = false;

    static u32 registered_events;

    template <typename EventT>
    static void register_event() {
        registered_events += 1;
        EventT::id = registered_events;
    }

    virtual ~event() = default;
    virtual id_t get_type() const = 0;
    virtual category get_category() const = 0;
};


#define EVENT_DECLARE(CATEGORY) \
    static core::event::id_t id; \
    static core::event::category get_static_category() { return CATEGORY; } \
    core::event::category get_category() const override { return get_static_category(); } \
    static core::event::id_t get_static_type() { return id; } \
    core::event::id_t get_type() const override { return get_static_type(); } \
    struct {} /* Declaration of event requires a semicolon at the end! */

} // core

#endif // GL2_SRC_CORE_EVENT_HPP
