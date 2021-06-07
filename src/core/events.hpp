#ifndef GL2_SRC_CORE_EVENTS_HPP
#define GL2_SRC_CORE_EVENTS_HPP

#include <defines.h>


namespace core {


enum class EventType {
    FRAME_FINISHED,
    PAUSE,
    STOP,
    RESTART,
    ZOOM,
    WINDOW_MOTION,
    TOGGLE_F2,
    TOGGLE_BODY_TRACES,
    TOGGLE_VECTOR_FIELD,
    TOGGLE_VELOCITIES,

    MOUSE_PRESS,
    MOUSE_RELEASE,
    MOUSE_MOVE,

    KEYBOARD_PRESS,
    KEYBOARD_RELEASE,

    CONTROLLER_PRESS,
    CONTROLLER_RELEASE,
    CONTROLLER_AXIS_MOVE,
};


struct IEvent {
    bool handled = false;

    virtual EventType get_type() const = 0;
};


#define DECLARE_EVENT_TYPE(TYPE) \
    static core::EventType get_static_type() { return TYPE; } \
    core::EventType get_type() const override { return get_static_type(); }


struct EventFrameFinished : public IEvent {
    f32 dt;

    EventFrameFinished(f32 dt_) :dt(dt_) {}

    DECLARE_EVENT_TYPE(EventType::FRAME_FINISHED);
};


struct EventPause : public IEvent {
    DECLARE_EVENT_TYPE(EventType::PAUSE);
};


struct EventStop : public IEvent {
    DECLARE_EVENT_TYPE(EventType::STOP);
};


struct EventRestart : public IEvent {
    DECLARE_EVENT_TYPE(EventType::RESTART);
};


struct EventZoom : public IEvent {
    DECLARE_EVENT_TYPE(EventType::ZOOM);

    f32 factor;

    EventZoom(f32 f) :factor(f) {}
};


struct WindowMotionEvent : public IEvent {
    DECLARE_EVENT_TYPE(EventType::WINDOW_MOTION);

    i32 x, y;
    WindowMotionEvent(i32 x, i32 y) :x(x), y(y) {}
};


struct EventToggleF2 : public IEvent {
    DECLARE_EVENT_TYPE(EventType::TOGGLE_F2);
};


struct EventToggleBodyTraces : public IEvent {
    DECLARE_EVENT_TYPE(EventType::TOGGLE_BODY_TRACES);
};


struct EventToggleVectorField : public IEvent {
    DECLARE_EVENT_TYPE(EventType::TOGGLE_VECTOR_FIELD);
};


struct EventToggleVelocities : public IEvent {
    DECLARE_EVENT_TYPE(EventType::TOGGLE_VELOCITIES);
};


} // core

#endif // GL2_SRC_CORE_EVENTS_HPP
