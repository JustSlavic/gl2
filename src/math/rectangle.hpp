#pragma once

#include "vector2.hpp"


namespace math {


// Rectangle with sides aligned to coordinate axis
struct rectangle {
    float32 top, left, bottom, right;

    inline f32 get_width() const {
        return math::abs(right - left);
    }

    inline f32 get_height() const {
        return math::abs(top - bottom);
    }

    inline vector2 get_top_left() const {
        return vector2{ left, top };
    }

    inline vector2 get_top_right() const {
        return vector2{ right, top };
    }

    inline vector2 get_bottom_left() const {
        return vector2{ left, bottom };
    }

    inline vector2 get_bottom_right() const {
        return vector2{ right, bottom };
    }

    inline bool is_empty() const {
        return math::is_zero(get_width()) || math::is_zero(get_height());
    }

    inline rectangle intersect(const rectangle& other) const {
        return {
            min(top, other.top),
            max(left, other.left),
            max(bottom, other.bottom),
            min(right, other.right)
        };
    }

    inline rectangle extend(const rectangle& other) const {
        return {
            max(top, other.top),
            min(left, other.left),
            min(bottom, other.bottom),
            max(right, other.right)
        };
    }
}


//
struct rotated_rectangle {
    vector2 top_left;
    vector2 top_right;
    vector2 bottom_right;
    vector2 bottom_left;
}


} // math
