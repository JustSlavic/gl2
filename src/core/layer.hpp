#ifndef GL2_SRC_CORE_LAYER_HPP
#define GL2_SRC_CORE_LAYER_HPP

#include <defines.hpp>
#include "dispatcher.hpp"
#include <modeling_2d/model.hpp>
#include <modeling_2d/camera.hpp>
#include <unordered_map>


namespace core {


struct ILayer : public event_receiver {
	virtual void draw() = 0;
	virtual void advance(f32 dt) = 0;
};


struct LayerSkybox : public ILayer {};


struct LayerWorld : public ILayer {
	Model model;
	Camera2D camera;

	math::matrix4 projection;

	Shader body_shader;
	Shader arrow_shader;

	VertexArray va;
	VertexBuffer vb;
	IndexBuffer ib;
	VertexBufferLayout vbl;

	math::vector2 camera_velocity_up_left = math::vector2::zero();
	math::vector2 camera_velocity_down_right = math::vector2::zero();

	LayerWorld();

	void draw() override;
	void advance(f32 dt) override;

	bool on_camera_move(core::event_camera_move* e);
	bool on_toggle_velocities(core::event_toggle_velocities* e);
	bool on_toggle_traces(core::event_toggle_traces* e);
	bool on_toggle_vector_field(core::event_toggle_vector_field* e);
	bool on_toggle_F2(core::event_toggle_F2* e);
};


struct LayerUI : public ILayer {};


struct LayerImGUI : public ILayer {};


} // core

#endif // GL2_SRC_CORE_LAYER_HPP
