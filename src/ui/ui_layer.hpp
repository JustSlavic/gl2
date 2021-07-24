#ifndef GL2_SRC_UI_LAYER_HPP
#define GL2_SRC_UI_LAYER_HPP

#include <core/layer.hpp>
#include "ui_shape.hpp"
#include <math.hpp>


namespace ui {


struct ui_layer : public core::layer {
    std::shared_ptr<shape> ui_root;
    math::matrix4 ui_transform;

    void initialize();
    void terminate();

    void draw() override;
    void advance(f32 dt) override;
};


} // ui

#ifdef UNITY_BUILD
#include "ui_layer.cpp"
#endif

#endif
