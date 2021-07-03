#include "ui_shape.hpp"

#include <graphics/vertex_array.h>
#include <graphics/vertex_buffer.h>
#include <graphics/vertex_buffer_layout.h>
#include <graphics/index_buffer.h>
#include <graphics/shader.h>
#include <graphics/renderer.h>

#include "ui_renderer.hpp"


namespace ui {


bool shape::hit_test (math::vector2 p_in_parent) {
    // Transform from parent's coordinates to local coordinates
    math::matrix4 view = get_transform_matrix();
    math::vector4 p_in_local = view * math::vector4::make(p_in_parent, 0, 1);

    // Check if p in local lies within local rectangle
    math::rectangle local_bounds{0, 0, size.x, size.y};
    return local_bounds.contains(p_in_local.xy);
}


void container::draw (const math::matrix4& parent_matrix) {
    auto transform = get_transform_matrix() * parent_matrix;

    for (auto& p_shape : childs) {
        p_shape->draw(transform);
    }
}


bool container::hit_test (math::vector2 p_in_parent) {
    for (auto& p_shape : childs) {
        bool success = p_shape->hit_test(p_in_parent);
        if (success) return true;
    }
    return false;
}


void rectangle::draw (const math::matrix4& parent_matrix) {
    // printf("Draw rectangle (%5.2f, %5.2f) -- (%5.2f, %5.2f)\n",
    //     position.x, position.y,
    //     position.x + size.x, position.y + size.y);

    auto top_left = position;
    auto bottom_right = position + size;

    renderer::draw_rectangle(math::rectangle(top_left, bottom_right));
    // renderer::draw_rectangle(math::rectangle(0, 0, 100, 20));

    // math::vector2 vertices[4] = {
    //     math::vector2{position.x,          position.y},
    //     math::vector2{position.x + size.x, position.y},
    //     math::vector2{position.x + size.x, position.y + size.y},
    //     math::vector2{position.x,          position.y + size.y},
    // };

    // for (math::vector2& v : vertices) {
    //     auto w = parent_matrix * math::vector4::make(v, 0, 1);
    //     v = w.xy;
    // }

    // static const u32 indices[] = {  // note that we start from 0!
    //     0, 1, 2,
    //     2, 3, 0,
    // };

    // VertexBuffer vb(vertices, sizeof(vertices) * 2);
    // IndexBuffer  ib(indices, sizeof(indices) / sizeof(u32));
    // VertexBufferLayout vbl;
    // VertexArray va;

    // vbl.push<f32>(2);
    // va.add_buffer(vb, vbl);

    // gl2::Renderer::draw(va, ib, *p_ui_shader);
}


bool rectangle::hit_test (math::vector2 p_in_parent) {
    return false;
}


void circle::draw (const math::matrix4& parent_view) {

}


bool circle::hit_test (math::vector2 p_in_parent) {
    return false;
}


} // ui
