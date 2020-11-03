#include "graphics_api.h"
#include <defines.h>


namespace gl2 {

    struct GraphicsApi::Impl {
        static GraphicsApi::Name get_api_name() {
            return GraphicsApi::Name::OpenGL;
        }
    };

    GraphicsApi &GraphicsApi::instance() {
        static GraphicsApi instance_;
        return instance_;
    }

    void GraphicsApi::startup() {
        instance().impl = new Impl();
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
