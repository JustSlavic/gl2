#pragma once

#include <defines.h>
#include <es/event_system.h>
#include <api/mouse.h>
// #include <api/keyboard.h>


struct EventMoveUp {};
struct EventMoveDown {};
struct EventMoveLeft {};
struct EventMoveRight {};

struct Keymap : public IEmitter {
    static Keymap &instance();

    template <typename T> 
    void update(T) {
        ASSERT2(false, "You must instantiate update function by hand");
    }

private:
    Keymap();
};
