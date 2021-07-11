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
    auto transform_ = get_transform_matrix();
    auto transform = parent_matrix * transform_;

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
    auto transform_ = get_transform_matrix();
    auto transform = parent_matrix * transform_;

    auto top_left = position;
    auto bottom_right = position + size;

    renderer::draw_rectangle(math::rectangle(top_left, bottom_right), transform, math::color32::unpack(math::packed_color<math::color_layout::ARGB>(0xFFAB8FE2)));
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
