#ifndef GIR1_RENDERER_H
#define GIR1_RENDERER_H

#include "vertex_array.h"
#include "index_buffer.h"
#include "shader.h"

//#include <glm/glm.hpp>


namespace gl2 {
    struct Renderer {
        static void init();
        static void clear(const math::color24& color);
        static void clear(const math::color32& color);
        static void draw(const VertexArray &vertex_array, const IndexBuffer &index_buffer, Shader &shader);
        static void draw(const VertexArray &vertex_array, Shader &shader);
    };
}

#ifdef UNITY_BUILD
#include "renderer.cpp"
#endif

#endif // GIR1_RENDERER_H
