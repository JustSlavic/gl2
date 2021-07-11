#include "ui_renderer.hpp"

#include <graphics/vertex_array.h>
#include <graphics/vertex_buffer.h>
#include <graphics/vertex_buffer_layout.h>
#include <graphics/index_buffer.h>
#include <graphics/shader.h>
#include <graphics/renderer.h>

namespace ui {


static Shader* p_ui_shader = nullptr;


void renderer::draw_rectangle(const math::rectangle& rect, const math::matrix4& transform, const math::color32& color) {
    if (p_ui_shader == nullptr) {
        p_ui_shader = new Shader();

        p_ui_shader->load_shader(Shader::Type::Vertex, "resources/shaders/ui.vs")
            .load_shader(Shader::Type::Fragment, "resources/shaders/ui.fs")
            .compile()
            .bind();
    }
    
    auto rect_size = rect.get_size();

    auto tl = (transform * math::vector4::make(0, 0, 0, 1)).xy;
    auto br = (transform * math::vector4::make(rect_size, 0, 1)).xy;

    math::vector2 vertices[4] = {
        math::vector2{ tl.x, tl.y }, // top left
        math::vector2{ br.x, tl.y }, // top right
        math::vector2{ br.x, br.y }, // bottom right
        math::vector2{ tl.x, br.y }, // bottom left
    };

    static const u32 indices[] = {  // note that we start from 0!
        0, 1, 2,
        2, 3, 0,
    };

    VertexBuffer vb(vertices, sizeof(vertices) * 2);
    IndexBuffer  ib(indices, sizeof(indices) / sizeof(u32));
    VertexBufferLayout vbl;
    VertexArray va;

    vbl.push<f32>(2);
    va.add_buffer(vb, vbl);
    p_ui_shader->set_uniform_4f("u_color", color);

    gl2::Renderer::draw(va, ib, *p_ui_shader);
}


} // ui

