#include <iostream>
#include <application.h>
#include <version.h>
#include <logging/logging.h>


int main(int argc, char** argv, char** env) {
    LogGlobalContext::instance()
        .set_level(Log::Level::Info)
        .attach(std::cout);
        // .attach(std::string("log/") + now_as_string() + ".log");

    LOG_INFO << "Welcome to Gir2 v." << version;

    gl2::gravity_simulation_app app;

    if (app.initialize()) {
        LOG_ERROR << "Failed to initialize some context!";
        std::exit(1);
    }

    return app.run();
}
