#ifndef GL2_ES_EMITTER_H
#define GL2_ES_EMITTER_H

#include "dispatcher.h"


struct IEmitter {
    template <typename TEvent>
    void emit(TEvent event) {
        Dispatcher<TEvent>::instance().emit(event);
    }

    template <typename TEvent, typename... Args>
    void emit(Args... args) {
        Dispatcher<TEvent>::instance().emit(TEvent(std::forward<Args...>(args)...));
    }
};


#endif // GL2_ES_EMITTER_H
