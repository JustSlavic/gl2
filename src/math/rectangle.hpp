#pragma once

#include "vector2.hpp"


namespace math {


// Rectangle with sides aligned to coordinate axis
struct rectangle {
private:
    float32 top, left, bottom, right;

public:
    rectangle(f32 t, f32 l, f32 b, f32 r)
        : top(t), left(l), bottom(b), right(r)
    {}

    rectangle(vector2 tl, vector2 br)
        : top(tl.y), left(tl.x), bottom(br.y), right(br.x)
    {}

    inline f32 get_width() const { return abs(right - left); }
    inline f32 get_height() const { return abs(top - bottom); }

    inline vector2 get_size() const { return vector2{ get_width(), get_height() }; }

    inline f32 get_top() const { return top; }
    inline f32 get_left() const { return left; }
    inline f32 get_right() const { return right; }
    inline f32 get_bottom() const { return bottom; }

    inline vector2 get_top_left() const { return vector2{ left, top }; }
    inline vector2 get_top_right() const { return vector2{ right, top }; }
    inline vector2 get_bottom_left() const { return vector2{ left, bottom }; }
    inline vector2 get_bottom_right() const { return vector2{ right, bottom }; }

    inline bool is_empty() const {
        return is_zero(get_width()) || is_zero(get_height());
    }

    inline bool contains(vector2 p) {
        return left <= p.x && p.x <= right
            && bottom <= p.y && p.y <= top;
    }

    inline rectangle intersect(const rectangle& other) const {
        return rectangle(
            min(top, other.top),
            max(left, other.left),
            max(bottom, other.bottom),
            min(right, other.right)
        );
    }

    inline rectangle extend(const rectangle& other) const {
        return rectangle(
            max(top, other.top),
            min(left, other.left),
            min(bottom, other.bottom),
            max(right, other.right)
        );
    }
};


//
struct rotated_rectangle {
    vector2 top_left;
    vector2 top_right;
    vector2 bottom_right;
    vector2 bottom_left;
};


} // math
