#pragma once

#include <vector>
#include <defines.h>
#include <graphics/shader.h>
#include <graphics/vertex_array.h>
#include <graphics/index_buffer.h>
#include <es/event_system.h>

#include <math/vector2.hpp>


struct body {
    math::vector2 position;
    math::vector2 velocity;
    f32 m;

    body(f32 x, f32 y);
    body(const math::vector2& p);
    body(f32 x, f32 y, f32 vx, f32 vy, f32 m);
    body(math::vector2 p, math::vector2 v, f32 m);
};


struct EventSelectedBodyMoved {
    math::vector2 body_position;

    EventSelectedBodyMoved(math::vector2 p) : body_position(p) {}
};


struct Model : public IEmitter {
    std::vector<body> bodies;
    std::vector<body> bodies_buffer;

    std::vector<f32> radii;
    std::vector<f32> radii_buffer;

    std::vector<std::vector<math::vector2>> traces;
    std::vector<std::vector<math::vector2>> traces_buffer;

    math::vector2 mouse_position;
    i32 selected_body_index = -1;

    // body cursor_highlight;

    Shader* shader = nullptr;
    VertexArray* va = nullptr;
    IndexBuffer* ib = nullptr;

    Shader* arrow_shader = nullptr;

    bool pause = false;
    bool elastic = false;
    bool draw_body_traces = false;
    bool draw_velocities = false;
    bool draw_vector_field = false;

    Model();

    void add_body(math::vector2 position, math::vector2 velocity, f32 mass);
    void draw_bodies();
    void move_bodies(f32 dt);
    void clean(EventRestart);
    void toggle_body_traces(EventToggleBodyTraces);
    void toggle_velocities(EventToggleVelocities);
    void toggle_vector_field(EventToggleVectorField);
    void on_mouse_move(math::vector2 position) { mouse_position = position; }
    void on_left_mouse_click(math::vector2 position);
    void on_right_mouse_click(math::vector2 position);
};
