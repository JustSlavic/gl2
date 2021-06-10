#ifndef GL2_SRC_CORE_INPUT_DEVICE_MANAGER_HPP
#define GL2_SRC_CORE_INPUT_DEVICE_MANAGER_HPP

#include "devices.hpp"
#include <vector>
#include <memory>
#include <algorithm>


namespace core::input {


struct device_manager {
    std::vector<std::shared_ptr<device>> devices;

    // SDL does not give device id for key_press key_release events.
    // Does this mean it can be only one keyboard? I will make only one
    // keyboard for now. Since it not going to the array, it doesn't matter
    // what device_id I will give it. Hopefully it will be sufficient. 
    //                                             - jslavic 10.06.21
    keyboard active_keyboard{ 0 };

    std::weak_ptr<mouse> active_mouse;
    std::weak_ptr<gamepad> active_gamepad;

public:
    template <typename Device>
    static void register_device(device::id_t id) {
        instance().devices.push_back(std::make_shared<Device>(id));
    }

    static void unregister_device(device::id_t id);

    static void keyboard_press(keyboard::key k);
    static void keyboard_release(keyboard::key k);

    static void mouse_press(device::id_t id, mouse::key k);
    static void mouse_release(device::id_t id, mouse::key k);
    static void mouse_scroll(device::id_t id, i32 amount);
    static void mouse_move(device::id_t id, i32 x, i32 y);

    static void gamepad_press(device::id_t id, gamepad::key k);
    static void gamepad_release(device::id_t id, gamepad::key k);
    static void gamepad_axis_move(device::id_t id, gamepad::axis a, f32 value);

private:
    device_manager() = default;
    device_manager(const device_manager&) = delete;
    device_manager(device_manager&&) = delete;

    static device_manager& instance();

    inline bool is_active(device::id_t id) {
        return active_mouse.lock() || active_gamepad.lock();
    }

    template <typename Device>
    std::shared_ptr<Device> get_device(device::id_t id) {
        static_assert(std::is_base_of<device, Device>::value, "Passed type have to be heir of device.");

        for (auto& device : devices) {
            if (device->get_category() == Device::get_static_category() &&
                device->get_id() == id)
            {
                return std::static_pointer_cast<Device>(device);
            }
        }

        auto p = std::make_shared<Device>(id);
        devices.push_back(p);

        return p;
    }

    std::shared_ptr<device> get_device(device::id_t id);
};




} // core::input


#endif // GL2_SRC_CORE_INPUT_DEVICE_MANAGER_HPP
