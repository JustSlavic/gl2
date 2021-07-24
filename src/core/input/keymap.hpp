#ifndef GL2_SRC_CORE_KEYMAP_HPP
#define GL2_SRC_CORE_KEYMAP_HPP

#include <string>
#include <unordered_map>
#include <core/input.hpp>


namespace core::input {


struct keymap {
	// Maps name of a key
	std::unordered_map<keyboard::key, std::string> keyboard_mapping;
	std::unordered_map<mouse::key, std::string> mouse_mapping;

    void activate();

	void activate(keyboard::key k);
	void deactivate(keyboard::key k);
	void activate(mouse::key k);
};


keymap load_keymap(const std::string& filename);


} // core::input

#ifdef UNITY_BUILD
#include "keymap.cpp"
#endif

#endif // GL2_SRC_CORE_KEYMAP_HPP
