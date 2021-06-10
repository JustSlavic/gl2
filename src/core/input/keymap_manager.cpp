#include "keymap_manager.hpp"
#include "keymap.hpp"


namespace core::input {


keymap_manager& keymap_manager::instance() {
    static keymap_manager instance_;
    return instance_;
}


void keymap_manager::register_keymap(std::string name) {
    // This is bullshit, but let me figure this out first,
    // then I will fix minor details.
    keymap* km = new keymap(load_keymap(std::string("resources/keymaps/") + name + std::string(".son")));
    instance().mappings.emplace(name, km);
    instance().active_keymap = km;
}


void keymap_manager::activate_keymap(std::string name) {
    auto found = instance().mappings.find(name);
    if (found != instance().mappings.end()) {
        instance().active_keymap = found->second;
    }
}


} // core::input

