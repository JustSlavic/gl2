#include "keymap.h"
#include <api/mouse.h>
#include <api/keyboard.h>


template<> 
void Keymap::update<Mouse::ButtonPressEvent>(Mouse::ButtonPressEvent event) {
    emit(EventMoveDown());

    if (event.button == Mouse::LEFT) {
        if (Mouse::get_state(Mouse::RIGHT) == Mouse::ButtonState::PRESSED) {
            LOG_INFO << "RIGHT->LEFT PRESSED";
        }
    }

    if (event.button == Mouse::RIGHT) {
        if (Mouse::get_state(Mouse::LEFT) == Mouse::ButtonState::PRESSED) {
            LOG_INFO << "LEFT->RIGHT PRESSED";
        }
    }
}

template<> 
void Keymap::update<Mouse::ButtonReleaseEvent>(Mouse::ButtonReleaseEvent event) {
    emit(EventMoveUp());
}

template<> 
void Keymap::update<Mouse::ScrollEvent>(Mouse::ScrollEvent event) {
    constexpr f32 ZOOM_SCALE = .01f;
    emit(EventZoom(ZOOM_SCALE * event.scroll));
}

template<>
void Keymap::update<Keyboard::KeyReleaseEvent>(Keyboard::KeyReleaseEvent event) {
    switch (event.key) {
        case Keyboard::ESC: emit(EventStop()); break;
        case Keyboard::R: emit(EventRestart()); break;
        case Keyboard::SPACE: emit(EventPause()); break;
        case Keyboard::F2: emit(EventToggleF2()); break;
        default: break;
    }
}

Keymap::Keymap() {
    Dispatcher<Mouse::ButtonPressEvent>::subscribe(EVENT_CALLBACK(update));
    Dispatcher<Mouse::ButtonReleaseEvent>::subscribe(EVENT_CALLBACK(update));
    Dispatcher<Mouse::ScrollEvent>::subscribe(EVENT_CALLBACK(update));
    Dispatcher<Keyboard::KeyReleaseEvent>::subscribe(EVENT_CALLBACK(update));
}

Keymap &Keymap::instance() {
    static Keymap instance_;
    return instance_;
}

