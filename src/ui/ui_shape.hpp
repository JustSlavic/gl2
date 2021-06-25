#ifndef GL2_SRC_UI_UI_SHAPE_HPP
#define GL2_SRC_UI_UI_SHAPE_HPP

#include <math.hpp>


namespace ui {


struct shape {
    // Pivot is the origin of the local coordinate system in parent's coordinates.
    math::vector2 pivot;

    // Position is the top left corner of the shape in local coordinate system.
    math::vector2 position;

    // Size is the width and height of the object's bounding box in local coordinates.
    math::vector2 size;

    // Rotation of the local coordinate system around Z axis clock-wise (accoding to the right-hand rule).
    f32 rotation;
};


struct rectangle : public shape {

}



} // ui


#endif // GL2_SRC_UI_UI_SHAPE_HPP
