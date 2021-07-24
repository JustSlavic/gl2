#ifndef GIR1_INDEX_BUFFER_H
#define GIR1_INDEX_BUFFER_H

#include <defines.hpp>


struct IndexBuffer {
    u32 id;
    u32 count;

    IndexBuffer(const u32* data, u64 count);
    ~IndexBuffer();

    void bind() const;
    void unbind() const;
};

#ifdef UNITY_BUILD
#include "index_buffer.cpp"
#endif

#endif // GIR1_INDEX_BUFFER_H
