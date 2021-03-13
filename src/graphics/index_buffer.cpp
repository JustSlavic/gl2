#include "index_buffer.h"
#include <utils.h>
#include <GL/glew.h>


IndexBuffer::IndexBuffer(const u32* data, u64 count) :count(count) {
    static_assert(sizeof(u32) == sizeof(GLuint), "oops");

    // give me 1 buffer and store its id
    glGenBuffers(1, &id); GL_CHECK_ERRORS;
    // LOG_DEBUG << "glGenBuffers(1, &id); -> " << id;

    // select this buffer (I'm about to work with it)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id); GL_CHECK_ERRORS;
    // LOG_DEBUG << "glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, " << id << ");";

    // fill in the buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(u32), data, GL_STATIC_DRAW); GL_CHECK_ERRORS;
    // LOG_DEBUG << "glBufferData(GL_ELEMENT_ARRAY_BUFFER, " 
              // << count*sizeof(u32) << ", data, GL_STATIC_DRAW);";
}


IndexBuffer::~IndexBuffer() {
    glDeleteBuffers(1, &id); GL_CHECK_ERRORS;
    // LOG_DEBUG << "glDeleteBuffers(1, " << id << ");";
}


void IndexBuffer::bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id); GL_CHECK_ERRORS;
    // LOG_DEBUG << "Bound index array " << id;
}


void IndexBuffer::unbind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); GL_CHECK_ERRORS;
    // LOG_DEBUG << "glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); <- " << id;
}
