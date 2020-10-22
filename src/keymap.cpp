#include "keymap.h"


template<> 
void Keymap::update<MouseButtonDownEvent>(MouseButtonDownEvent event) {
    emit(EventMoveDown());
}

template<> 
void Keymap::update<MouseButtonUpEvent>(MouseButtonUpEvent event) {
    emit(EventMoveUp());
}

Keymap::Keymap() {
    Dispatcher<MouseButtonDownEvent>::subscribe(EVENT_CALLBACK(update));
    Dispatcher<MouseButtonUpEvent>::subscribe(EVENT_CALLBACK(update));
}

Keymap &Keymap::instance() {
    static Keymap instance_;
    return instance_;
}

