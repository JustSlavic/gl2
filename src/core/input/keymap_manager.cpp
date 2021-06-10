#include "keymap_manager.hpp"


namespace core::input {


keymap_manager& keymap_manager::instance() {
    static keymap_manager instance_;
    return instance_;
}


void keymap_manager::register_keymap(std::string name) {
    load_keymap(std::string("resources/keymaps/") + name + std::string(".son"));
}


void keymap_manager::activate_keymap(std::string name) {
    auto found = instance().mappings.find(name);
    if (found != instance().mappings.end()) {
        instance().active_keymap = found->second;
    }
}


} // core::input

