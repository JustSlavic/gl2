#ifndef GIR1_VERTEX_BUFFER_H
#define GIR1_VERTEX_BUFFER_H

#include <defines.h>


struct VertexBuffer {
    u32 id;

    VertexBuffer(const void* data, u64 size);
    ~VertexBuffer();

    void bind() const;
    void unbind() const;
};


#endif // GIR1_VERTEX_BUFFER_H
