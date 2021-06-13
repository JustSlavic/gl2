#ifndef GL2_SRC_CORE_LAYER_HPP
#define GL2_SRC_CORE_LAYER_HPP

#include "dispatcher.hpp"
#include <modeling_2d/model.hpp>
#include <modeling_2d/camera.hpp>
#include <unordered_map>


namespace core {


struct ILayer : public EventReceiver {
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

	LayerWorld();
	void draw() override;
	void advance(f32 dt) override;

	//inline bool on_gamepad_button_pressed(Gamepad_XBox::Event_ButtonPressed* pEvent) {
	//	printf("XBOX button pressed: %d\n", (i32)pEvent->button);
	//	return true;
	//}

	//inline bool on_gamepad_axis_changed(Gamepad_XBox::Event_AxisChanged* pEvent) {
	//	printf("XBOX axis changed: %d (%f -> %f)\n", (i32)pEvent->axis, pEvent->old_value, pEvent->new_value);
	//	return true;
	//}

	//bool on_mouse_move(Mouse::MoveEvent* pEvent);
	//bool on_mouse_press(Mouse::ButtonPressEvent* pEvent);

	/*inline bool on_frame_finished(EventFrameFinished* pEvent) {
		model.move_bodies(pEvent->dt);
		return false;
	}*/
};


struct LayerUI : public ILayer {};


struct LayerImGUI : public ILayer {};


} // core

#endif // GL2_SRC_CORE_LAYER_HPP
