#include "keymap.hpp"
#include <events.hpp>
#include <son.hpp>
#include <memory>


namespace core::input {


std::shared_ptr<event> get_down_event_by_name(std::string name) {
    if (name == "move_up") return std::make_shared<event_camera_move>(event_camera_move::direction::up, event_camera_move::type::start);
    if (name == "move_down") return std::make_shared<event_camera_move>(event_camera_move::direction::down, event_camera_move::type::start);
    if (name == "move_left") return std::make_shared<event_camera_move>(event_camera_move::direction::left, event_camera_move::type::start);
    if (name == "move_right") return std::make_shared<event_camera_move>(event_camera_move::direction::right, event_camera_move::type::start);
    
    return {};
}


std::shared_ptr<event> get_up_event_by_name(std::string name) {
    if (name == "exit") return std::make_shared<event_exit>();

    if (name == "move_up") return std::make_shared<event_camera_move>(event_camera_move::direction::up, event_camera_move::type::stop);
    if (name == "move_down") return std::make_shared<event_camera_move>(event_camera_move::direction::down, event_camera_move::type::stop);
    if (name == "move_left") return std::make_shared<event_camera_move>(event_camera_move::direction::left, event_camera_move::type::stop);
    if (name == "move_right") return std::make_shared<event_camera_move>(event_camera_move::direction::right, event_camera_move::type::stop);

    if (name == "toggle_velocities") return std::make_shared<event_toggle_velocities>();
    if (name == "toggle_traces") return std::make_shared<event_toggle_traces>();
    if (name == "toggle_vector_field") return std::make_shared<event_toggle_vector_field>();
    if (name == "toggle_F2") return std::make_shared<event_toggle_F2>();
    
    return {};
}


keyboard::key get_keyboard_key_by_name(std::string k) {
    if (k == "ESC") return keyboard::ESC;
    if (k == "W") return keyboard::W;
    if (k == "A") return keyboard::A;
    if (k == "S") return keyboard::S;
    if (k == "D") return keyboard::D;
    if (k == "T") return keyboard::T;
    if (k == "V") return keyboard::V;
    if (k == "F") return keyboard::F;

    return keyboard::KEYBOARD_KEY_MAX;
}


void keymap::activate() {
    //keyboard* kb = &device_manager::instance().active_keyboard;

    //for (keyboard::key k = (keyboard::key)0; k < keyboard::KEYBOARD_KEY_MAX; k = (keyboard::key)((i32)k + 1)) {
    //    if (kb->is_pressed(k)) {
    //        auto it = keyboard_mapping.find(k);
    //        if (it != keyboard_mapping.end()) {
    //            auto e = get_down_event_by_name(it->second);
    //            core::event_system::emit(e);
    //        }
    //    }
    //}
}


void keymap::activate(keyboard::key k) {
    auto it = keyboard_mapping.find(k);
    if (it != keyboard_mapping.end()) {
        auto e = get_down_event_by_name(it->second);
        core::event_system::emit(e);
    }
}


void keymap::deactivate(keyboard::key k) {
    auto it = keyboard_mapping.find(k);
    if (it != keyboard_mapping.end()) {
        auto e = get_up_event_by_name(it->second);
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
