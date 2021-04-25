#pragma once

#include <vector>
#include <defines.h>
#include <graphics/shader.h>
#include <graphics/vertex_array.h>
#include <graphics/index_buffer.h>

#include <math/vector2.hpp>


struct body {
    math::vector2 position;
    f32 vx, vy;
    f32 m;

    body(f32 x, f32 y);
    body(const math::vector2& p);
    body(f32 x, f32 y, f32 vx, f32 vy, f32 m);
    body(const math::vector2& p, f32 vx, f32 vy, f32 m);
};

struct Model {
    std::vector<body> bodies;
    std::vector<body> buffer;

    // body cursor_highlight;

    Shader* shader = nullptr;
    VertexArray* va;
    IndexBuffer* ib;

    Shader* arrow_shader = nullptr;

    bool pause = false;
    bool elastic = false;

    Model();

    void add_body(f32 x, f32 y);
    void draw_bodies();
    void move_bodies(f32 dt);
    void clean();
};
