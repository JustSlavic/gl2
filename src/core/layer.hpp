#ifndef GL2_SRC_CORE_LAYER_HPP_2
#define GL2_SRC_CORE_LAYER_HPP_2

#include "dispatcher.hpp"
#include "keymap2.hpp"
#include <modeling_2d/model.hpp>
#include <unordered_map>
#include <api/gamepad_xbox.hpp>


namespace core {


struct ILayer : public EventReceiver {
	virtual void draw() = 0;
};


struct LayerSkybox : public ILayer {};


struct LayerWorld : public ILayer {
	Model model;

	std::shared_ptr<Keymap2> active_keymap;
	std::vector<Keymap2> keymaps;

	LayerWorld() {
		Keymap2 km = load_keymap("resources/keymappings/gravity_keys.son");
		keymaps.push_back(km);

		bind<Gamepad_XBox::Event_ButtonPressed, LayerWorld, &LayerWorld::on_gamepad_button_pressed>(this);
		bind<Gamepad_XBox::Event_AxisChanged, LayerWorld, &LayerWorld::on_gamepad_axis_changed>(this);
	}

	void draw() override;

	inline bool on_gamepad_button_pressed(Gamepad_XBox::Event_ButtonPressed* pEvent) {
		printf("XBOX button pressed: %d\n", (i32)pEvent->button);
		return true;
	}

	inline bool on_gamepad_axis_changed(Gamepad_XBox::Event_AxisChanged* pEvent) {
		printf("XBOX axis changed: %d (%f -> %f)\n", (i32)pEvent->axis, pEvent->old_value, pEvent->new_value);
		return true;
	}
};


struct LayerUI : public ILayer {};


struct LayerImGUI : public ILayer {};


} // core

#endif // GL2_SRC_CORE_LAYER_HPP
