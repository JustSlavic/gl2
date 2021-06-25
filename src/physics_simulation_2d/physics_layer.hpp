#ifndef GL2_SRC_PHYSICS_SIMULATION_2D_PHYSICS_LAYER_HPP
#define GL2_SRC_PHYSICS_SIMULATION_2D_PHYSICS_LAYER_HPP

#include <core/layer.hpp>
#include "model.hpp"


namespace physics_simulation_2d {


struct layer_world : public core::layer {
	model m_model;

	void draw() override;
	void advance(f32 dt) override;
};


}; // physics_simulation_2d


#endif // GL2_SRC_PHYSICS_SIMULATION_2D_PHYSICS_LAYER_HPP
