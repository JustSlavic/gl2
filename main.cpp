#include <iostream>
#include <utils.h>
#include <version.h>
#include <application.h>
#include <logging/logging.h>

#include <stdarg.h>
#include <son/value.hpp>


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
    SON::Value v_obj = {
        std::pair<const char*, std::nullptr_t>{ "null", nullptr },
        { "foo", 1 },
        { "bar", 50.123 },
        { "baz", true }
    };
    
    //for (auto& val : v_obj) {
    //    if (val.is_null()) printf("null\n");
    //    if (val.is_integer()) printf("for OBJ: %llu\n", val.get_integer());
    //    if (val.is_boolean()) printf("for OBJ: %s\n", val.get_boolean() ? "true" : "false");
    //    if (val.is_floating()) printf("for OBJ: %lf\n", val.get_floating());
    //}

    SON::Value v_arr = { 1, 2, nullptr, 4, "foo" };

    //for (auto& val : v_arr) {
    //    if (val.is_integer()) printf("for ARR: %llu\n", val.get_integer());
    //    if (val.is_boolean()) printf("for ARR: %s\n", val.get_boolean() ? "true" : "false");
    //    if (val.is_floating()) printf("for ARR: %lf\n", val.get_floating());
    //    if (val.is_string()) printf("some string???\n");
    //}

    //return 0;

    print2("Hello {} world {}\n", 1, 2);

    LogGlobalContext::instance()
        .set_level(Log::Level::Info)
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
