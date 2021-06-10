#ifndef GL2_SRC_CORE_EVENT_HPP
#define GL2_SRC_CORE_EVENT_HPP

#include <defines.hpp>


namespace core {


struct event {
    enum class category {
        FRAME_EVENT,
        INPUT_EVENT,
        ACTION_EVENT,
    };

    enum class type {
        FRAME_FINISHED,

        ACTION_EXIT,

        PAUSE,
        RESTART,
        ZOOM,
        WINDOW_MOTION,
        TOGGLE_F2,
        TOGGLE_BODY_TRACES,
        TOGGLE_VECTOR_FIELD,
        TOGGLE_VELOCITIES,

        MOUSE_PRESS,
        MOUSE_RELEASE,
        MOUSE_SCROLL,
        MOUSE_MOVE,

        KEYBOARD_PRESS,
        KEYBOARD_RELEASE,

        CONTROLLER_PRESS,
        CONTROLLER_RELEASE,
        CONTROLLER_AXIS_MOVE,
    };

    bool handled = false;

    virtual type get_type() const = 0;
    virtual category get_category() const = 0;
};


#define EVENT_DECLARE_TYPE(TYPE) \
    static core::event::type get_static_type() { return TYPE; } \
    core::event::type get_type() const override { return get_static_type(); } \
    struct {} /* Declaration of event type requires a semicolon! */


#define EVENT_DECLARE_CATEGORY(CAT) \
    static core::event::category get_static_category() { return CAT; } \
    core::event::category get_category() const override { return get_static_category(); } \
    struct {} /* Declaration of event category requires a semicolon! */


// struct EventFrameFinished : public IEvent {
//     f32 dt;

//     EventFrameFinished(f32 dt_) :dt(dt_) {}

//     DECLARE_EVENT_CATEGORY(EventCategory::FRAME_EVENT);
//     DECLARE_EVENT_TYPE(EventType::FRAME_FINISHED);
// };


// struct EventPause : public IEvent {
//     DECLARE_EVENT_CATEGORY(EventCategory::ACTION_EVENT);
//     DECLARE_EVENT_TYPE(EventType::PAUSE);
// };


// struct EventStop : public IEvent {
//     DECLARE_EVENT_CATEGORY(EventCategory::ACTION_EVENT);
//     DECLARE_EVENT_TYPE(EventType::STOP);
// };


// struct EventRestart : public IEvent {
//     DECLARE_EVENT_CATEGORY(EventCategory::ACTION_EVENT);
//     DECLARE_EVENT_TYPE(EventType::RESTART);
// };


// struct EventZoom : public IEvent {
//     DECLARE_EVENT_CATEGORY(EventCategory::ACTION_EVENT);
//     DECLARE_EVENT_TYPE(EventType::ZOOM);

//     f32 factor;

//     EventZoom(f32 f) :factor(f) {}
// };


// struct WindowMotionEvent : public IEvent {
//     DECLARE_EVENT_CATEGORY(EventCategory::ACTION_EVENT);
//     DECLARE_EVENT_TYPE(EventType::WINDOW_MOTION);

//     i32 x, y;
//     WindowMotionEvent(i32 x, i32 y) :x(x), y(y) {}
// };


// struct EventToggleF2 : public IEvent {
//     DECLARE_EVENT_CATEGORY(EventCategory::ACTION_EVENT);
//     DECLARE_EVENT_TYPE(EventType::TOGGLE_F2);
// };


// struct EventToggleBodyTraces : public IEvent {
//     DECLARE_EVENT_CATEGORY(EventCategory::ACTION_EVENT);
//     DECLARE_EVENT_TYPE(EventType::TOGGLE_BODY_TRACES);
// };


// struct EventToggleVectorField : public IEvent {
//     DECLARE_EVENT_CATEGORY(EventCategory::ACTION_EVENT);
//     DECLARE_EVENT_TYPE(EventType::TOGGLE_VECTOR_FIELD);
// };


// struct EventToggleVelocities : public IEvent {
//     DECLARE_EVENT_CATEGORY(EventCategory::ACTION_EVENT);
//     DECLARE_EVENT_TYPE(EventType::TOGGLE_VELOCITIES);
// };


} // core

#endif // GL2_SRC_CORE_EVENT_HPP
