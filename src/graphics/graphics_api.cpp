#include "graphics_api.h"

#include <GL/glew.h>

#include <defines.h>
#include <logging/logging.h>


namespace gl2 {

    struct GraphicsApi::Impl {
        static GraphicsApi::Name get_api_name() {
            return GraphicsApi::Name::OpenGL;
        }

        i32 startup() {
            GLenum err = glewInit();
            if (GLEW_OK != err) {
                /* Problem: glewInit failed, something is seriously wrong. */
                LOG_ERROR << glewGetErrorString(err);
                return 1;
            }

            return 0;
        }
    };

    GraphicsApi &GraphicsApi::instance() {
        static GraphicsApi instance_;
        return instance_;
    }

    void GraphicsApi::startup() {
        auto& api = instance();
        api.impl = new Impl();

        i32 err = api.impl->startup();
        if (err) {
            LOG_ERROR << "Startup of " << to_string(api.get_api_name()) << " failed";
            std::exit(2);
        }

        LOG_INFO << "GLEW    v." << glewGetString(GLEW_VERSION);
        LOG_INFO << "OpenGL  v." << glGetString(GL_VERSION);
        LOG_INFO << "Renderer: " << glGetString(GL_RENDERER);
    }

    void GraphicsApi::shutdown() {
        delete instance().impl;
    }

    GraphicsApi::Name GraphicsApi::get_api_name() {
        return instance().impl->get_api_name();
    }

    const char *to_string(GraphicsApi::Name name) {
        switch (name) {
            case GraphicsApi::Name::None:     return "None";
            case GraphicsApi::Name::OpenGL:   return "OpenGL";
            case GraphicsApi::Name::Direct3D: return "Direct3D";
            case GraphicsApi::Name::Metal:    return "Metal";
            case GraphicsApi::Name::Vulkan:   return "Vulkan";
            default: ASSERT(false);
        }
        return nullptr;
    }

}
