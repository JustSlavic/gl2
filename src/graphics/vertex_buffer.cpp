#include "vertex_buffer.h"
#include <defines.h>
#include <logging/logging.h>

#include <GL/glew.h>


VertexBuffer::VertexBuffer(const void* data, u64 size) {
    // give me 1 buffer and store its id in &id
    glGenBuffers(1, &id); GL_CHECK_ERRORS;
    LOG_DEBUG << "glGenBuffers(1, &id); -> " << id;

    // select this buffer (I'm about to work with it)
    glBindBuffer(GL_ARRAY_BUFFER, id); GL_CHECK_ERRORS;
    LOG_DEBUG << "glBindBuffer(GL_ARRAY_BUFFER, " << id << ");";

    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW); GL_CHECK_ERRORS;
    LOG_DEBUG << "glBufferData(GL_ARRAY_BUFFER, " << size << ", data, GL_STATIC_DRAW); <- " << id;
}


VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &id); GL_CHECK_ERRORS;
    LOG_DEBUG << "glDeleteBuffers(1, " << id << ");";
}


void VertexBuffer::bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, id); GL_CHECK_ERRORS;
    // LOG_DEBUG << "Bound vertex buffer " << id;
}


void VertexBuffer::unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0); GL_CHECK_ERRORS;
    LOG_DEBUG << "glBindBuffer(GL_ARRAY_BUFFER, 0); <- " << id;
}
