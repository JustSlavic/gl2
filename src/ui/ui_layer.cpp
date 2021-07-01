#include "ui_layer.hpp"
#include "ui_shape_loader.hpp"


namespace ui {



void ui_layer::initialize() {
    ui_shape_loader loader;
    ui_root = loader.get_debug_ui();
}


void ui_layer::terminate() {

}


void ui_layer::draw() {
    auto identity = math::matrix4::identity();
    if (ui_root) ui_root->draw(identity);
}


void ui_layer::advance(f32 dt) {

}


} // ui


