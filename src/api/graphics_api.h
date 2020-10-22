#ifndef WM_GRAPHICS_API_H
#define WM_GRAPHICS_API_H


namespace gl2 {

    struct GraphicsApi {
        enum class Name {
            None,
            DirectX,
            Metal,
            OpenGL,
            Vulkan,
        };

        GraphicsApi();

        static Name getApiName();

        struct Impl;
        Impl* impl;
    };

}

#endif // WM_GRAPHICS_API_H

