#ifndef GL2_SRC_UI_SHAPE_LOADER_HPP
#define GL2_SRC_UI_SHAPE_LOADER_HPP

#include "ui_shape.hpp"


namespace ui {


// Shape loader loads ui elements from SON files.
//
// Rectangle:
// {
//   type = "rectangle"
//   position = vector2(0, 0)
//   size = vector2(100, 100)
//   scale = vector2(1, 1)
//   rotation_z = 0 // might be emited because it's default value
// }
//
//
struct ui_shape_loader {
public:
    int initialize();
    int terminate();

    std::shared_ptr<shape> load_ui_shape(const char* name);

    std::shared_ptr<shape> get_debug_ui();
};


} // ui


#endif // GL2_SRC_UI_SHAPE_LOADER_HPP
