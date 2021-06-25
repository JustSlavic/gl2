#ifndef GL2_SRC_CORE_DISPATCHER_HPP
#define GL2_SRC_CORE_DISPATCHER_HPP

#include "events.hpp"
#include <vector>
#include <tuple>


namespace core {


class Dispatcher {
    template<typename EventT, class C, bool(C::*M)(EventT*)>
    static bool invoke(void* instance, event* e) {
        if (EventT::get_static_type() == 0) return false;
        if (EventT::get_static_type() != e->get_type()) return false;
        return (static_cast<C*>(instance)->*M)(static_cast<EventT*>(e));
    }

public:
    template<typename EventT, class C, bool(C::*M)(EventT*)>
    void bind(C* instance) {
        auto pair = std::make_pair(&invoke<EventT, C, M>, instance);
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


struct event_receiver {
private:
    Dispatcher d;

public:
    template <typename EventT, typename Host, bool(Host::*M)(EventT*)>
    void bind(Host* host) {
        d.bind<EventT, Host, M>(host);
    }

    bool handle(event* e) {
        return d(e);
    }
};


} // core


#endif // GL2_SRC_CORE_DISPATCHER_HPP
