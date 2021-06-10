#ifndef GL2_SRC_CORE_DISPATCHER_HPP
#define GL2_SRC_CORE_DISPATCHER_HPP

#include "events.hpp"
#include <vector>
#include <tuple>


namespace core {


class Dispatcher {
    template<typename EventType, class C, bool(C::* M)(EventType*)>
    static bool invoke(void* instance, event* e) {
        if (EventType::get_static_type() != e->get_type()) return false;
        return (static_cast<C*>(instance)->*M)(static_cast<EventType*>(e));
    }

public:
    template<typename EventType, class C, bool(C::* M)(EventType*)>
    void bind(C* instance) {
        auto pair = std::make_pair(&invoke<EventType, C, M>, instance);
        targets.push_back(pair);
    }

    bool operator()(event* e) {
        for (auto pair : targets) {
            if ((pair.first)(pair.second, e)) {
                return true;
            }
        }

        return false;
    }

private:
    using Fn = bool(*)(void*, event*);
    std::vector<std::pair<Fn, void*>> targets;
};


struct EventReceiver {
    Dispatcher d;

    template <typename EventType, typename Host, bool(Host::*M)(EventType*)>
    void bind(Host* host) {
        d.bind<EventType, Host, M>(host);
    }

    bool handle(event* e) {
        return d(e);
    }
};


} // core


#endif // GL2_SRC_CORE_DISPATCHER_HPP
