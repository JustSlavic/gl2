#ifndef GL2_ES_DISPATCHER_H
#define GL2_ES_DISPATCHER_H

#include <vector>
#include <functional>


struct IObserver;

template <typename TEvent>
struct Dispatcher {
    using Callback = std::function<void(TEvent)>;
    std::vector<Callback> subscriptions;

    static Dispatcher& instance() {
        static Dispatcher instance;
        return instance;      
    }

    static void subscribe(Callback f) {
        Dispatcher::instance().subscriptions.push_back(f);
    }

    void emit(TEvent event) {
        for (auto f : subscriptions) {
            f(event);
        }  
    }

private:
    Dispatcher() = default;
};

#define SUBSCRIBE_TO_EVENT(T) \
        Dispatcher<T>::subscribe([this](T event) { this->update(event); });
#define EVENT_CALLBACK(F) [this](auto&&... args) -> decltype(auto) { this->F(std::forward<decltype(args)>(args)...); }

#endif // GL2_ES_DISPATCHER_H
