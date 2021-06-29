#include "ui_shape.hpp"


namespace ui {


bool shape::hit_test (math::vector2 p_in_parent) {
    // Transform from parent's coordinates to local coordinates
    math::matrix4 view = get_view_matrix();
    math::vector4 p_in_local = view * math::vector4{p_in_parent, 0, 1};

    // Check if p in local lies within local rectangle
    math::rectangle local_bounds{0, 0, size.x, size.y};
    return local_bounds.contains(p_in_local);
}


void container::draw (const math::matrix4& parent_matrix) {
    auto transform = get_transform_matrix() * parent_matrix;

    for (auto& p_shape : childs) {
        bool success = p_shape->draw(transform);
    }
}


bool container::hit_test (math::vector2 p_in_parent) {
    for (auto& p_shape : childs) {
        bool success = p_shape->hit_test(p_in_parent);
        if (success) return true;
    }
    return false;
}


void rectangle::draw (const math::matrix4& parent_view) {

}


bool rectangle::hit_test (math::vector2 p_in_parent) {

}


void circle::draw (const math::matrix4& parent_view) {

}


bool circle::hit_test (math::vector2 p_in_parent) {

}


} // ui
