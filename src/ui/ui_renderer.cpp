#include "ui_renderer.hpp"

#include <graphics/vertex_array.h>
#include <graphics/vertex_buffer.h>
#include <graphics/vertex_buffer_layout.h>
#include <graphics/index_buffer.h>
#include <graphics/shader.h>
#include <graphics/renderer.h>

#include <config.hpp>


namespace ui {


static Shader* p_ui_shader = nullptr;
static math::matrix4 ui_transform;


void renderer::draw_rectangle(const math::rectangle& rect) {    
    if (p_ui_shader == nullptr) {
        p_ui_shader = new Shader();

        p_ui_shader->load_shader(Shader::Type::Vertex, "resources/shaders/ui.vs")
            .load_shader(Shader::Type::Fragment, "resources/shaders/ui.fs")
            .compile()
            .bind();

        auto& cfg = config::get_instance();
        ui_transform = math::matrix4{
            2.f / cfg.window.width, 0, 0, -1.f,
            0, -2.f / cfg.window.height, 0, 1.f,
            0, 0, 1, 0,
            0, 0, 0, 1,
        };
    }

    math::vector2 vertices[4] = {
        (ui_transform * math::vector4::make(rect.get_top_left(), 0, 1)).xy,
        (ui_transform * math::vector4::make(rect.get_top_right(), 0, 1)).xy,
        (ui_transform * math::vector4::make(rect.get_bottom_right(), 0, 1)).xy,
        (ui_transform * math::vector4::make(rect.get_bottom_left(), 0, 1)).xy,
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

    gl2::Renderer::draw(va, ib, *p_ui_shader);
}


} // ui

