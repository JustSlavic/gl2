#ifndef GL2_APPLICATION_H
#define GL2_APPLICATION_H

#include <defines.hpp>
#include <api/window.h>
#include <events.hpp>
#include <logging/logging.h>
#include <core/dispatcher.hpp>
#include <vector>


namespace gl2 {


struct application : public core::event_receiver {
    Window* window = nullptr;
    bool initialized = false;
    bool running = false;
    std::vector<core::layer*> layers;

    application();
    ~application();

    virtual int initialize();
    virtual int terminate();

    int run();

private:
    bool on_exit(core::event_exit*);
};


struct gravity_simulation_app : public application {
    using super = application;

    int initialize() override;
    int terminate() override;
};


struct physics_engine_app : public application {
    using super = application;

    int initialize() override;
    int terminate() override;
};


}



#endif // GL2_APPLICATION_H
