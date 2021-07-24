#ifndef GIR1_VERTEX_BUFFER_H
#define GIR1_VERTEX_BUFFER_H

#include <defines.hpp>


struct VertexBuffer {
    u32 id;

    VertexBuffer(const void* data, u64 size);
    ~VertexBuffer();

    void bind() const;
    void unbind() const;
};

#ifdef UNITY_BUILD
#include "vertex_buffer.cpp"
#endif

#endif // GIR1_VERTEX_BUFFER_H
