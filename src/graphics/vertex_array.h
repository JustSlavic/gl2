#ifndef GIR1_VERTEX_ARRAY_H
#define GIR1_VERTEX_ARRAY_H


#include "vertex_buffer.h"
#include "vertex_buffer_layout.h"


struct VertexArray {
    u32 id;

    VertexArray();
    ~VertexArray();

    void add_buffer(const VertexBuffer& vb, const VertexBufferLayout& layout) const;

    void bind() const;
    void unbind() const;
};


#ifdef UNITY_BUILD
#include "vertex_array.cpp"
#endif

#endif // GIR1_VERTEX_ARRAY_H
