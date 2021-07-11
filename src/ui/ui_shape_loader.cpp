#include "ui_shape_loader.hpp"
#include <son.hpp>


namespace ui {


int ui_shape_loader::initialize() {
    return 0;
}


int ui_shape_loader::terminate() {
    return 0;
}


std::shared_ptr<container> load_ui_container(const jslavic::son& value);
std::shared_ptr<rectangle> load_ui_rectangle(const jslavic::son& value);


std::shared_ptr<container> load_ui_container(const jslavic::son& value) {
    if (value["type"].get_string() != "ui_container") return nullptr;

    auto result = std::make_shared<container>();

    if (value["position"].is_object()) {
        result->position.x = value["position"]["x"].get_integer();
        result->position.y = value["position"]["y"].get_integer();
    }

    if (value["size"].is_object()) {
        result->size.x = value["size"]["x"].get_integer();
        result->size.y = value["size"]["y"].get_integer();
    }

    if (value["scale"].is_object()) {
        result->scale.x = value["scale"]["x"].get_floating();
        result->scale.y = value["scale"]["y"].get_floating();
    }

    for (const auto& v : value["children"]) {
        if (v["type"].get_string() == "ui_rectangle") {
            auto rect = load_ui_rectangle(v);
            result->push(rect);
        }
        jslavic::pretty_print(v);
    }

    return result;
}


std::shared_ptr<rectangle> load_ui_rectangle(const jslavic::son& value) {
    if (value["type"].get_string() != "ui_rectangle") return nullptr;

    auto result = std::make_shared<rectangle>();

    if (value["position"].is_object()) {
        result->position.x = value["position"]["x"].get_integer();
        result->position.y = value["position"]["y"].get_integer();
    }

    if (value["size"].is_object()) {
        result->size.x = value["size"]["x"].get_integer();
        result->size.y = value["size"]["y"].get_integer();
    }

    if (value["scale"].is_object()) {
        result->scale.x = value["scale"]["x"].get_floating();
        result->scale.y = value["scale"]["y"].get_floating();
    }

    return result;
}


std::shared_ptr<shape> ui_shape_loader::load_ui_shape(const char* name) {
    jslavic::son value = jslavic::parse(std::string(name));

    if (value["type"] == "ui_rectangle") {
        return load_ui_rectangle(value);
    }
    if (value["type"] == "ui_container") {
        return load_ui_container(value);
    }

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

