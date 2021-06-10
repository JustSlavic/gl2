#ifndef GL2_SRC_CORE_INPUT_KEYMAP_MANAGER_HPP
#define GL2_SRC_CORE_INPUT_KEYMAP_MANAGER_HPP

#include "keymap.hpp"
#include <string>
#include <unordered_map>


namespace core::input {


struct keymap_manager {
    std::unordered_map<std::string, keymap*> mappings;
    keymap* active_keymap;

    static keymap_manager& instance();
    static void register_keymap(std::string name);
    static void activate_keymap(std::string name);

private:
    keymap_manager() = default;
    keymap_manager(const keymap_manager&) = delete;
    keymap_manager(keymap_manager&&) = delete;
};


} // core::input



#endif // GL2_SRC_CORE_INPUT_KEYMAP_MANAGER_HPP
