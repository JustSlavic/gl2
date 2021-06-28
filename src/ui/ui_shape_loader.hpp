#ifndef GL2_SRC_UI_SHAPE_LOADER_HPP
#define GL2_SRC_UI_SHAPE_LOADER_HPP

#include "ui_shape.hpp"


namespace ui {


struct ui_shape_loader {
public:
    int initialize();
    int terminate();

    shape load_ui_shape(const char* name);
};


} // ui


#endif // GL2_SRC_UI_SHAPE_LOADER_HPP
