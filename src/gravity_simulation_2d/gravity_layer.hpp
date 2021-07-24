#ifndef GL2_SRC_GRAVITY_SIMULATION_2D_GRAVITY_LAYER_HPP
#define GL2_SRC_GRAVITY_SIMULATION_2D_GRAVITY_LAYER_HPP


#include <core/layer.hpp>
#include "model.hpp"
#include "camera.hpp"
#include <graphics/shader.h>
#include <graphics/vertex_array.h>
#include <graphics/vertex_buffer.h>
#include <graphics/vertex_buffer_layout.h>
#include <graphics/index_buffer.h>


namespace gravity_simulation_2d {


struct layer_world : public core::layer {
    Model model;
    Camera2D camera;

    math::matrix4 projection;

    Shader body_shader;
    Shader arrow_shader;

    VertexArray va;
    VertexBuffer vb;
    IndexBuffer ib;
    VertexBufferLayout vbl;

    math::vector2 camera_velocity_up_left = math::vector2::zero;
    math::vector2 camera_velocity_down_right = math::vector2::zero;

    layer_world();

    void draw() override;
    void advance(f32 dt) override;

    bool on_camera_move(core::event_camera_move* e);
    bool on_toggle_velocities(core::event_toggle_velocities* e);
    bool on_toggle_traces(core::event_toggle_traces* e);
    bool on_toggle_vector_field(core::event_toggle_vector_field* e);
    bool on_toggle_F2(core::event_toggle_F2* e);
};


} // gravity_simulation_2d

#ifdef UNITY_BUILD
#include "gravity_layer.cpp"
#endif

#endif // GL2_SRC_GRAVITY_SIMULATION_2D_GRAVITY_LAYER_HPP
