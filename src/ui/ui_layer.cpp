#include "ui_layer.hpp"
#include "ui_shape_loader.hpp"


namespace ui {



void initialize() {
    ui_shape_loader loader;
    ui_root = loader.get_debug_ui();
}


void terminate() {

}


void ui_layer::draw() {
    if (ui_root) ui_root->draw();
}


void ui_layer::advance(f32 dt) {

}


} // ui


