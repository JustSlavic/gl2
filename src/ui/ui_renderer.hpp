#ifndef GL2_SRC_UI_RENDERER_HPP
#define GL2_SRC_UI_RENDERER_HPP

#include <math.hpp>


namespace ui {


struct renderer {
    static void draw_rectangle(const math::rectangle& rect, const math::matrix4& transform, const math::color32& color = math::color32::white);
};


} // ui


#ifdef UNITY_BUILD
#include "ui_renderer.cpp"
#endif

#endif // GL2_SRC_UI_RENDERER_HPP
