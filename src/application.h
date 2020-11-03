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

        template <typename T>
        void update(T) {
            ASSERT2(false, "You have to instantiate update(T) manually");
        }
    private:
        void on_stop(StopEvent);
    };
}



#endif // GL2_APPLICATION_H
