#include "renderer.h"
#include <logging/logging.h>


namespace gl2 {
    void Renderer::clear(const glm::vec3 &color) {
        glClearColor(color.r, color.g, color.b, 1.0); GL_CHECK_ERRORS;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
    }

    void Renderer::draw(const VertexArray &vertex_array, const IndexBuffer &index_buffer, Shader &shader) {
        vertex_array.bind();
        index_buffer.bind();
        shader.bind();

        glDrawElements(GL_TRIANGLES, index_buffer.count, GL_UNSIGNED_INT, nullptr); GL_CHECK_ERRORS;
        // LOG_DEBUG << "glDrawElements("
        //           << GL_TRIANGLES << ", " << index_buffer.count << ", " << GL_UNSIGNED_INT << ", " << "nullptr" << ");";
    }

    void Renderer::init() {
        glEnable(GL_BLEND); GL_CHECK_ERRORS;
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); GL_CHECK_ERRORS;
    }

    void Renderer::draw(const VertexArray &vertex_array, Shader &shader) {
        vertex_array.bind();
        shader.bind();

        // @Investigate: what is 0 and 36
        glDrawArrays(GL_TRIANGLES, 0, 36); GL_CHECK_ERRORS;
    }
}
