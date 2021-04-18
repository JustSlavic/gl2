#include <iostream>
#include <utils.h>
#include <version.h>
#include <application.h>
#include <logging/logging.h>

#include <config.hpp>

#include <stdarg.h>


template <typename... Args>
int log(const char* fmt, Args... args) {
    return 0;
}

int print2(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    while (*fmt) {
        if (*fmt == '{' and *(fmt+1) == '}') {
            int i = va_arg(args, int);
            printf("%d", i);
            fmt++;
        } else {
            printf("%c", *fmt);
        }

        fmt++;
    }

    va_end(args);

    return 0;
}


int main(int argc, char** argv, char** env) {
    print2("Hello {} world {}\n", 1, 2);

    LogGlobalContext::instance()
        .set_level(Log::Level::Warning)
        .attach(std::cout);
        // .attach(std::string("log/") + now_as_string() + ".log");

    LOG_INFO << "Welcome to Gir2 v." << version;

    bool config_initialized = config::initialize("config.son");
    if (not config_initialized) {
        printf("Could not initialize config!\n");
        return 1;
    }

    auto& cfg = config::get_instance();
    LOG_WARNING << "WINDOW: " << cfg.window.width << "x" << cfg.window.height;

    gl2::Application app;
    if (app.initialize()) {
        LOG_ERROR << "Failed to initialize some context!";
        std::exit(1);
    }
    return app.run();
}
