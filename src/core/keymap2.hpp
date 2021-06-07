#ifndef GL2_SRC_CORE_KEYMAP_HPP
#define GL2_SRC_CORE_KEYMAP_HPP


#include <api/keyboard.h>
#include <api/mouse.h>
#include <son.hpp>
#include <unordered_map>


namespace core {


struct Action {

};

struct Keymap2 {
	std::unordered_map<Keyboard::Key, Action> keyboard_mapping;
	std::unordered_map<Mouse::Button, Action> mouse_mapping;
};


core::Keymap2 load_keymap(const std::string& filename);


} // core


#endif // GL2_SRC_CORE_KEYMAP_HPP
