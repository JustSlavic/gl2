#ifndef GL2_SRC_CORE_KEYMAP_HPP
#define GL2_SRC_CORE_KEYMAP_HPP

#include <string>


namespace core::input {


struct keymap {};


keymap load_keymap(const std::string& filename);


} // core::input


#endif // GL2_SRC_CORE_KEYMAP_HPP
