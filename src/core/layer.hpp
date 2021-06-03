#ifndef GL2_SRC_CORE_LAYER_HPP_2
#define GL2_SRC_CORE_LAYER_HPP_2

#include "dispatcher.hpp"
#include <modeling_2d/model.hpp>


namespace core {


struct LayerSkybox : public EventReceiver {};


struct LayerWorld : public EventReceiver {
	Model model;
};


struct LayerUI : public EventReceiver {};


struct LayerImGUI : public EventReceiver {};


} // core

#endif // GL2_SRC_CORE_LAYER_HPP
