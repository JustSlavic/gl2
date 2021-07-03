#ifndef GL2_SRC_UI_UI_SHAPE_HPP
#define GL2_SRC_UI_UI_SHAPE_HPP

#include <math.hpp>
#include <vector>
#include <memory>


namespace ui {


struct shape {
public:
    // Pivot is the origin of the local coordinate system in parent's coordinates.
    math::vector2 pivot = math::vector2::zero;

    // Position is the top left corner of the shape in local coordinate system.
    math::vector2 position = math::vector2::zero;

    // Size is the width and height of the object's bounding box in local coordinates.
    math::vector2 size = math::vector2::zero;

    // Scale is the multiply coefficients, that dictate size transformation of the shape.
    math::vector2 scale = math::vector2::one;

    // Rotation of the local coordinate system around Z axis clock-wise (accoding to the right-hand rule).
    f32 rotation_z = 0.f;

public:
    virtual ~shape () = default;

    // Combines all the parent's view matrices and passes all of them
    // to its children, to draw everything that lies 'below'.
    virtual void draw (const math::matrix4& parent_view) = 0;

    math::matrix4 get_transform_matrix () const {
        return {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1,
        };
    }

    // p is in parent's coordinate system
    virtual bool hit_test (math::vector2 p_in_parent);
};


// Container is the group of different ui objects.
struct container : public shape {
private:
    // Owning pointers to child objects.
    std::vector<std::shared_ptr<shape>> childs;

public:
    void draw (const math::matrix4& parent_matrix) override;
    bool hit_test (math::vector2 p_in_parent) override;
    void push (std::shared_ptr<shape> child) { childs.push_back(child); }
};


struct rectangle : public shape {
private:
public:
    void draw (const math::matrix4& parent_view) override;
    bool hit_test (math::vector2 p_in_parent) override;
};


struct circle : public shape {
private:
public:
    void draw (const math::matrix4& parent_view) override;
    bool hit_test (math::vector2 p_in_parent) override;
};


} // ui


#endif // GL2_SRC_UI_UI_SHAPE_HPP