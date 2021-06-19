#include "device_manager.hpp"
#include "keymap_manager.hpp"
#include "keymap.hpp"


namespace core::input {


device_manager& device_manager::instance() {
    static device_manager instance_;
    return instance_;
}


void device_manager::unregister_device(device::id_t id) {
    auto& manager = device_manager::instance();

    manager.devices.erase(
        std::remove_if(
            manager.devices.begin(),
            manager.devices.end(),
            [&](std::shared_ptr<device>& device) { return device->get_id() == id; }),
        manager.devices.end());
}


std::shared_ptr<device> device_manager::get_device(device::id_t id) {
    auto& manager = device_manager::instance();

    for (auto& device : manager.devices) {
        if (device && device->get_id() == id) { return device; }
    }

    return nullptr;
}


void device_manager::keyboard_press(keyboard::key k) {
    printf("keyboard press\n");
    device_manager::instance().active_keyboard.press(k);
    keymap* km = keymap_manager::instance().active_keymap;
    if (km != nullptr) {
        km->activate(k);
    }
}


void device_manager::keyboard_release(keyboard::key k) {
    printf("keyboard release\n");
    device_manager::instance().active_keyboard.release(k);
    keymap* km = keymap_manager::instance().active_keymap;
    if (km != nullptr) {
        km->deactivate(k);
    }
}


void device_manager::mouse_press(device::id_t id, mouse::key k) {
    auto& manager = instance();

    auto p_device = manager.get_device<mouse>(id);
    p_device->press(k);

    manager.active_mouse = p_device;
}


void device_manager::mouse_release(device::id_t id, mouse::key k) {
    auto& manager = instance();

    auto p_device = manager.get_device<mouse>(id);
    p_device->release(k);

    manager.active_mouse = p_device;
}


void device_manager::mouse_scroll(device::id_t id, i32 amount) {
    auto& manager = instance();

    auto p_device = manager.get_device<mouse>(id);
    p_device->scroll(amount);

    manager.active_mouse = p_device;
}


void device_manager::mouse_move(device::id_t id, i32 x, i32 y) {
    auto& manager = instance();

    auto p_device = manager.get_device<mouse>(id);
    p_device->move(x, y);

    manager.active_mouse = p_device;
}


void device_manager::gamepad_press(device::id_t id, gamepad::key k) {
    auto& manager = instance();

    auto p_device = manager.get_device<gamepad>(id);
    p_device->press(k);

    manager.active_gamepad = p_device;
}


void device_manager::gamepad_release(device::id_t id, gamepad::key k) {
    auto& manager = instance();

    auto p_device = manager.get_device<gamepad>(id);
    p_device->release(k);

    manager.active_gamepad = p_device;
}


void device_manager::gamepad_axis_move(device::id_t id, gamepad::axis a, f32 value) {
    auto& manager = instance();
    
    auto p_device = manager.get_device<gamepad>(id);
    p_device->move_axis(a, value);

    manager.active_gamepad = p_device;
}

} // core::input
