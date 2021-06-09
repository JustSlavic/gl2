#ifndef GL2_APPLICATION_H
#define GL2_APPLICATION_H

#include <defines.h>
#include <api/window.h>
#include <events.hpp>
#include <logging/logging.h>
#include <core/dispatcher.hpp>
#include <vector>


namespace gl2 {
    struct application : public core::EventReceiver {
        Window* window = nullptr;
        bool running = false;
        std::vector<core::ILayer*> layers;

        application();
        ~application();

        int initialize();
        int run();

    private:
        bool on_stop(core::EventStop*);
    };
}



#endif // GL2_APPLICATION_H
