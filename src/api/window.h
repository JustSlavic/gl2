#ifndef GL2_WM_WINDOW_H
#define GL2_WM_WINDOW_H

#include <defines.hpp>


namespace gl2 {

struct Window {
    Window(i32 width, i32 height);
    Window(i32 width, i32 height, const char* title);
    ~Window();

    i32 initialize();
    void terminate();

    i32 get_width();
    i32 get_height();

    void poll_events();
    void swap_buffers();

private:
    struct Impl;
    Impl *impl;
};

}


#ifdef UNITY_BUILD
#include "window.cpp"
#endif

#endif // GL2_WM_WINDOW_H
