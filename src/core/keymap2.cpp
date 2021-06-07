#include "keymap2.hpp"

namespace core {


core::Keymap2 load_keymap(const std::string& filename) {
    jslavic::son value = jslavic::parse(filename);

    auto keyboard = value["keyboard"];
    if (keyboard.is_object()) {
        for (auto p : keyboard.pairs()) {
            std::string& k = p.first;
            auto& v = p.second;

            printf("Keyboard mapping: %s => %s\n", v.get_string().c_str(), k.c_str());
        }
    }

    auto mouse = value["mouse"];
    if (mouse.is_object()) {
        for (auto p : mouse.pairs()) {
            std::string& k = p.first;
            auto& v = p.second;

            printf("Mouse mapping: %s => %s\n", v.get_string().c_str(), k.c_str());
        }
    }

    return core::Keymap2();
}


} // core
