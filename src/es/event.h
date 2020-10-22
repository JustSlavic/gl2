#ifndef GL2_ES_EVENT_H
#define GL2_ES_EVENT_H

#include <defines.h>


struct StopEvent {};

struct WindowMotionEvent {
    i32 x, y;
    WindowMotionEvent(i32 x, i32 y) :x(x), y(y) {}
};

struct MouseMotionEvent {
    i32 x, y;
    MouseMotionEvent(i32 x, i32 y) :x(x), y(y) {}
};

#endif // GL2_ES_EVENT_H
