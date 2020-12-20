#ifndef GIR1_VERTEX_BUFFER_LAYOUT_H
#define GIR1_VERTEX_BUFFER_LAYOUT_H

#include <vector>
#include <GL/glew.h>
#include <defines.h>


struct LayoutElement {
    u32 count;
    u32 type;
    u32 normalized;

    constexpr static u32 size_of_gl_type(u32 type) {
        switch (type) {
            case GL_FLOAT:
                return sizeof(GLfloat);
            case GL_UNSIGNED_INT:
                return sizeof(GLuint);
            case GL_UNSIGNED_BYTE:
                return sizeof(GLbyte);
            default:
                ASSERT(false);
                return -1;
        }
    }
};


struct VertexBufferLayout {
    std::vector<LayoutElement> elements;
    u32 stride = 0;

    template <typename T>
    void push(u32 count) {
        ASSERT(false);
    }

    inline u32 get_stride() const { return stride; }
};

template <>
void VertexBufferLayout::push<f32>(u32 count);

template <>
void VertexBufferLayout::push<u32>(u32 count);

template <>
void VertexBufferLayout::push<u8>(u32 count);

#endif // GIR1_VERTEX_BUFFER_LAYOUT_H
