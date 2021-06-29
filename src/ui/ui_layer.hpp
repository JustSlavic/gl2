#ifndef GL2_SRC_UI_LAYER_HPP
#define GL2_SRC_UI_LAYER_HPP

#include <core/layer.hpp>
#include "ui_shape.hpp"


namespace ui {


struct ui_layer : public core::layer {
    std::shared_ptr<shape> ui_root;

    void initialize();
    void terminate();

    void draw() override;
    void advance(f32 dt) override;
};


} // ui


#endif
