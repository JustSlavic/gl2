#include <defines.hpp>

#include <iostream>
#include <application.h>
#include <version.h>
#include <logging/logging.h>

#ifdef PLATFORM_WINDOWS
#include <windows.h>
#endif


#ifdef PLATFORM_LINUX
int main(int argc, char** argv, char** env) {
    LogGlobalContext::instance()
        .set_level(Log::Level::Info)
        .attach(std::cout);
        // .attach(std::string("log/") + now_as_string() + ".log");

    LOG_INFO << "Welcome to Gir2 v." << version;

    gl2::physics_engine_app app;

    if (app.initialize()) {
        LOG_ERROR << "Failed to initialize some context!";
        std::exit(1);
    }

    return app.run();
}
#endif


#ifdef PLATFORM_WINDOWS
int WINAPI
WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{
    const char* msg = "Hello world!\n";

    OutputDebugStringA(msg);
    LogGlobalContext::instance()
        .set_level(Log::Level::Info)
        .attach(std::cout);
    // .attach(std::string("log/") + now_as_string() + ".log");

    LOG_INFO << "Welcome to Gir2 v." << version;

    gl2::physics_engine_app app;

    if (app.initialize()) {
        LOG_ERROR << "Failed to initialize some context!";
        std::exit(1);
    }

    return app.run();
    //return 0;
}
#endif
