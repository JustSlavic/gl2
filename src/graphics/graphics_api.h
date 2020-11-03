#ifndef WM_GRAPHICS_API_H
#define WM_GRAPHICS_API_H


namespace gl2 {

    struct GraphicsApi {
        enum class Name {
            None,
            OpenGL,
            Direct3D,
            Metal,
            Vulkan,
        };

        static void startup();
        static void shutdown();

        static Name get_api_name();

    private:
        GraphicsApi() = default;
        static GraphicsApi &instance();

        struct Impl;
        Impl* impl;
    };

    const char *to_string(GraphicsApi::Name);
}

#endif // WM_GRAPHICS_API_H

