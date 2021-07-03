#include "ui_layer.hpp"
#include "ui_shape_loader.hpp"
#include <config.hpp>


namespace ui {


void ui_layer::initialize() {
    auto& cfg = config::get_instance();

    ui_shape_loader loader;
    ui_root = loader.load_ui_shape("resources/ui/root.ui.son"); // loader.get_debug_ui();

    ui_transform = math::matrix4{
        1.f / cfg.window.width, 0, 0, -1.f,
        0, -1.f / cfg.window.height, 0, 1.f,
        0, 0, 1, 0,
        0, 0, 0, 1,
    };
}


void ui_layer::terminate() {

}


void ui_layer::draw() {
    if (ui_root) ui_root->draw(ui_transform);
}


void ui_layer::advance(f32 dt) {

}


} // ui


