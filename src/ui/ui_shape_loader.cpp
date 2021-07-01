#include "ui_shape_loader.hpp"
#include <son.hpp>


namespace ui {


int ui_shape_loader::initialize() {
    return 0;
}


int ui_shape_loader::terminate() {
    return 0;
}


std::shared_ptr<shape> ui_shape_loader::load_ui_shape(const char* name) {
    jslavic::son value = jslavic::parse(std::string(name) + ".son");
    return nullptr;
}


std::shared_ptr<shape> ui_shape_loader::get_debug_ui() {
    auto rect = std::make_shared<ui::rectangle>();
    rect->size = math::vector2{ 100.f, 100.f };

    auto ui_root = std::make_shared<ui::container>();
    ui_root->push(rect);

    return ui_root;
}


} // ui

