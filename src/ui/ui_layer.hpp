#ifndef GL2_SRC_UI_LAYER_HPP
#define GL2_SRC_UI_LAYER_HPP

#include <core/layer.hpp>


namespace ui {


struct ui_layer : public core::layer {
    void draw() override;
    void advance(f32 dt) override;
};


} // ui


#endif
