#pragma once

#include <defines.h>
#include <es/event_system.h>


struct EventMoveUp {};
struct EventMoveDown {};
struct EventMoveLeft {};
struct EventMoveRight {};

/*
 *  This class provides means to remap key presses to actions
 *  @TODO: make reading keymaps from file to make setting saving possible
 */
struct Keymap : public IEmitter {
    static Keymap &instance();

    template <typename T> 
    void update(T) {
        ASSERT2(false, "You must instantiate update function by hand");
    }

private:
    Keymap();
};
