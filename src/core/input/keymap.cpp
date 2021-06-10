#include "keymap.hpp"
#include <events.hpp>
#include <son.hpp>
#include <memory>


namespace core::input {


std::shared_ptr<event> get_event_by_name(std::string name) {
    if (name == "exit") return std::make_shared<event_exit>();

    return {};
}

keyboard::key get_keyboard_key_by_name(std::string k) {
    if (k == "ESC") return keyboard::ESC;

    return keyboard::KEYBOARD_KEY_MAX;
}


void keymap::activate(keyboard::key k) {
    auto it = keyboard_mapping.find(k);
    if (it != keyboard_mapping.end()) {
        auto e = get_event_by_name(it->second);
        core::event_system::emit(e);
    }
}


void keymap::activate(mouse::key k) {

}


keymap load_keymap(const std::string& filename) {
    jslavic::son value = jslavic::parse(filename);

    keymap km;

    auto keyboard = value["keyboard"];
    if (keyboard.is_object()) {
        for (auto p : keyboard.pairs()) {
            std::string action = p.first;
            std::string k = p.second.get_string();

            printf("Keyboard mapping: %s => %s\n", action.c_str(), k.c_str());
            km.keyboard_mapping.emplace(get_keyboard_key_by_name(k), action);
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

    return km;
}


} // core::input
