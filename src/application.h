#ifndef GL2_APPLICATION_H
#define GL2_APPLICATION_H

#include <defines.h>
#include <api/window.h>
#include <events.hpp>
#include <logging/logging.h>
#include <core/dispatcher.hpp>
#include <vector>


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

    struct Application_2 : public core::EventReceiver {
        Window* window = nullptr;
        bool running = false;
        std::vector<core::ILayer*> layers;

        Application_2();
        ~Application_2();

        int initialize();
        int run();

    private:
        bool on_stop(core::EventStop*);
    };
}



#endif // GL2_APPLICATION_H
