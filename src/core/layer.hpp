#ifndef GL2_SRC_CORE_LAYER_HPP
#define GL2_SRC_CORE_LAYER_HPP

#include <defines.hpp>
#include <core/dispatcher.hpp>


namespace core {

struct layer : public event_receiver {
	virtual void draw () = 0;
	virtual void advance (f32 dt) = 0;
};

} // core

#endif // GL2_SRC_CORE_LAYER_HPP
