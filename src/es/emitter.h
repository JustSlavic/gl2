#ifndef GL2_ES_EMITTER_H
#define GL2_ES_EMITTER_H

#include "dispatcher.h"


struct IEmitter {
    template <typename TEvent>
    void emit(TEvent event) {
        Dispatcher<TEvent>::instance().emit(event);
    }
};


#endif // GL2_ES_EMITTER_H
