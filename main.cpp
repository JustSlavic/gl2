#include <iostream>
#include <utils.h>
#include <version.h>
#include <application.h>
#include <logging/logging.h>

#include <config.hpp>


int main(int argc, char** argv, char** env) {
    LogGlobalContext::instance()
        .set_level(Log::Level::Warning)
        .attach(std::cout);
        // .attach(std::string("log/") + now_as_string() + ".log");

    LOG_INFO << "Welcome to Gir2 v." << version;

    config::initialize("config.son");

    auto& cfg = config::get_instance();
    LOG_WARNING << "WINDOW: " << cfg.window.width << "x" << cfg.window.height;

    gl2::Application app;
    if (app.initialize()) {
        LOG_ERROR << "Failed to initialize some context!";
        std::exit(1);
    }
    return app.run();
}
