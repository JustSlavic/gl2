#pragma once

#include <vector>
#include <defines.h>
#include <graphics/shader.h>
#include <graphics/vertex_array.h>
#include <graphics/index_buffer.h>

#include <math/vec2.hpp>


struct Circle {
    f32 x, y;
    f32 vx, vy;
    f32 m;

    Circle(f32 x, f32 y);
    Circle(f32 x, f32 y, f32 vx, f32 vy, f32 m);
};

struct Model {
    std::vector<Circle> bodies;
    std::vector<Circle> buffer;

    Shader* shader;
    VertexArray* va;
    IndexBuffer* ib;

    bool pause = false;
    bool elastic = false;

    Model();

    void add_body(f32 x, f32 y);
    void draw_bodies();
    void move_bodies(f32 dt);
    void clean();
};
