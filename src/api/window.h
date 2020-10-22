#ifndef GL2_WM_WINDOW_H
#define GL2_WM_WINDOW_H

#include "graphics_api.h"

namespace gl2 {

struct Window {
    Window(int width, int height);
    Window(int width, int height, const char* title);
    ~Window();

    int startup();
    void shutdown();

    void poll_events();
    void swap_buffers();

private:
    struct Impl;
    Impl *impl;
};

}


#endif // GL2_WM_WINDOW_H
