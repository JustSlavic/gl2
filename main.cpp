#include <iostream>
#include <utils.h>
#include <version.h>
#include <application.h>
#include <logging/logging.h>


int main(int argc, char** argv, char** env) {
    LogGlobalContext::instance()
        .attach(std::cout);
        // .attach(std::string("log/") + now_as_string() + ".log");

    LOG_INFO << "Welcome to Gir2 v." << version;

    gl2::Application app;
    if (app.initialize()) {
        LOG_ERROR << "Failed to initialize some context!";
        std::exit(1);
    }
    return app.run();
}
