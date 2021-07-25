#ifndef GL2_SRC_PHYSICS_SIMULATION_2D_PHYSICS_LAYER_HPP
#define GL2_SRC_PHYSICS_SIMULATION_2D_PHYSICS_LAYER_HPP

#include <core/layer.hpp>
#include "model.hpp"
#include <graphics/shader.h>
#include <graphics/vertex_array.h>
#include <graphics/vertex_buffer.h>
#include <graphics/vertex_buffer_layout.h>
#include <graphics/index_buffer.h>
#include <gravity_simulation_2d/camera.hpp>


namespace physics_simulation_2d {


struct layer_world : public core::layer {
	model m_model;
    Camera2D m_camera;

    Shader body_shader;
    Shader arrow_shader;

    VertexArray va;
    VertexBuffer vb;
    IndexBuffer ib;
    VertexBufferLayout vbl;

    math::matrix4 projection;

public:
    layer_world();
	void draw() override;
	void advance(f32 dt) override;
};


}; // physics_simulation_2d


#ifdef UNITY_BUILD
#include "physics_layer.cpp"
#endif

#endif // GL2_SRC_PHYSICS_SIMULATION_2D_PHYSICS_LAYER_HPP
