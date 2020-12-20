#include "vertex_array.h"
#include <logging/logging.h>


VertexArray::VertexArray() {
    glGenVertexArrays(1, &id); GL_CHECK_ERRORS;
    LOG_DEBUG << "glGenVertexArrays(1, &id); -> " << id;
}

VertexArray::~VertexArray() {
    glDeleteVertexArrays(1, &id); GL_CHECK_ERRORS;
    LOG_DEBUG << "glDeleteVertexArrays(1, " << id << ");";
}

void VertexArray::add_buffer(const VertexBuffer &vb, const VertexBufferLayout &layout) const {
    bind();
    vb.bind();

    u64 offset = 0;

    for (u32 i = 0; i < layout.elements.size(); ++i) {
        auto& element = layout.elements[i];

        glVertexAttribPointer(
                /*index*/          i,
                /*size of vertex*/ element.count,
                /*type*/           element.type,
                /*normalize?*/     element.normalized,
                /*stride*/         layout.get_stride(),
                /*pointer*/        (void*) offset);
        GL_CHECK_ERRORS;
        LOG_DEBUG << "glVertexAttribPointer(" 
                  << "index=" << i << ", "
                  << "size=" << element.count << ", "
                  << "type=" << element.type << ", "
                  << "normalize?=" << element.normalized << ", "
                  << "stride=" << layout.get_stride() << ", "
                  << "pointer=" << offset << ");";

        glEnableVertexAttribArray(i); GL_CHECK_ERRORS;
        LOG_DEBUG << "glEnableVertexAttribArray(" << i << ");";

        offset += element.count * LayoutElement::size_of_gl_type(element.type);
    }
}

void VertexArray::bind() const {
    glBindVertexArray(id); GL_CHECK_ERRORS;
    // LOG_DEBUG << "glBindVertexArray(" << id << ");";
}

void VertexArray::unbind() const {
    glBindVertexArray(0); GL_CHECK_ERRORS;
    LOG_DEBUG << "glBindVertexArray(0);";
}
