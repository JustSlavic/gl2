#include "layer.hpp"
#include <config.hpp>


namespace core {


static const f32 vertices[] = {
    0.5f,  0.5f, 0.0f,  // top right
    0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left
};

static const u32 indices[] = {  // note that we start from 0!
    0, 1, 3,  // first Triangle
    1, 2, 3   // second Triangle
};

constexpr f32 NEAR_CLIP_DISTANCE = 0.1f;
constexpr f32 FAR_CLIP_DISTANCE = 1000.f;


LayerWorld::LayerWorld() {
    auto& cfg = config::get_instance();
    f32 window_ratio = cfg.window.width / cfg.window.height;
    projection = math::projection(window_ratio * NEAR_CLIP_DISTANCE, NEAR_CLIP_DISTANCE, NEAR_CLIP_DISTANCE, FAR_CLIP_DISTANCE);

    Keymap2 km = load_keymap("resources/keymappings/gravity_keys.son");
    keymaps.push_back(km);

    bind<Gamepad_XBox::Event_ButtonPressed, LayerWorld, &LayerWorld::on_gamepad_button_pressed>(this);
    bind<Gamepad_XBox::Event_AxisChanged, LayerWorld, &LayerWorld::on_gamepad_axis_changed>(this);
}


void LayerWorld::draw() {
    
}


} // core

