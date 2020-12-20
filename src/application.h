#ifndef GL2_APPLICATION_H
#define GL2_APPLICATION_H

#include <defines.h>
#include <api/window.h>
#include <es/event_system.h>
#include <logging/logging.h>


namespace gl2 {
    struct Application : public IEmitter {
        Window* window{nullptr};
        bool running = false;

        Application();
        ~Application();

        int initialize();
        int run();

    private:
        void on_stop(EventStop);
    };
}



#endif // GL2_APPLICATION_H
