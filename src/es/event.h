#ifndef GL2_ES_EVENT_H
#define GL2_ES_EVENT_H

#include <defines.h>


struct EventFrameFinished {
    f32 dt;

    EventFrameFinished(f32 dt_) :dt(dt_) {}
};

struct EventPause {};
struct EventStop {};
struct EventRestart {};

struct EventZoom {
    f32 factor;

    EventZoom(f32 f) :factor(f) {}
};

struct WindowMotionEvent {
    i32 x, y;
    WindowMotionEvent(i32 x, i32 y) :x(x), y(y) {}
};

struct EventToggleF2 {};

struct EventToggleBodyTraces {};

struct EventToggleVectorField {};

struct EventToggleVelocities {};

#endif // GL2_ES_EVENT_H
