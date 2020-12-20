#include "vertex_buffer_layout.h"


template <>
void VertexBufferLayout::push<f32>(u32 count) {
    elements.push_back({count, GL_FLOAT, GL_FALSE});
    stride += count*LayoutElement::size_of_gl_type(GL_FLOAT);
}

template <>
void VertexBufferLayout::push<u32>(u32 count) {
    elements.push_back({count, GL_UNSIGNED_INT, GL_FALSE});
    stride += count*LayoutElement::size_of_gl_type(GL_UNSIGNED_INT);
}

template <>
void VertexBufferLayout::push<u8>(u32 count) {
    elements.push_back({count, GL_UNSIGNED_BYTE, GL_TRUE});
    stride += count*LayoutElement::size_of_gl_type(GL_UNSIGNED_BYTE);
}
