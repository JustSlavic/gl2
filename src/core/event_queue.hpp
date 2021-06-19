#ifndef GL2_SRC_CORE_EVENT_QUEUE_HPP
#define GL2_SRC_CORE_EVENT_QUEUE_HPP


#include "events.hpp"
#include <deque>
#include <mutex>
#include <condition_variable>
#include <memory>


namespace core {


struct EventQueue {
    std::deque<std::shared_ptr<event>> queue;
    std::mutex m_mutex;
    std::condition_variable cv;


    static EventQueue& instance() {
        static EventQueue instance_;
        return instance_;
    }


    static bool empty() {
        return instance().queue.empty();
    }


    template <typename EventType, typename... Args>
    static void push_event(Args&&... args) {
        auto& q = instance();
        std::lock_guard lock(q.m_mutex);

        q.queue.push_back(std::make_shared<EventType>(std::forward<Args>(args)...));
        q.cv.notify_one();
    }


    template <typename EventType>
    static void push_event() {
        auto& q = instance();
        std::lock_guard lock(q.m_mutex);

        q.queue.push_back(std::make_shared<EventType>());
        q.cv.notify_one();
    }


    static void push_event(std::shared_ptr<event> e) {
        if (!e) return;

        auto& q = instance();
        std::lock_guard lock(q.m_mutex);

        q.queue.push_back(e);
        q.cv.notify_one();
    }


    static std::shared_ptr<event> get_event() {
        auto& queue = instance();
        std::shared_ptr<event> result = nullptr;

        {
            std::unique_lock lock(queue.m_mutex);
            queue.cv.wait(lock, [&queue]() { return !queue.queue.empty(); });

            result = queue.queue.front();
        }

        return result;
    }


    static void pop_event() {
        auto& queue = instance();

        {
            std::unique_lock lock(queue.m_mutex);
            queue.cv.wait(lock, [&queue]() { return !queue.queue.empty(); });

            queue.queue.pop_front();
        }
    }

private:
    EventQueue() = default;
    EventQueue(const EventQueue&) = delete;
    EventQueue(EventQueue&&) = delete;
};


namespace event_system {

inline void emit(std::shared_ptr<event> e) {
    EventQueue::push_event(e);
}

template <typename EventType, typename... Args>
void emit(Args&&... args) {
    EventQueue::push_event<EventType>(std::forward<Args>(args)...);
}

template <typename EventType>
void emit() {
    EventQueue::push_event<EventType>();
}

} // event_system


} // core



#endif // GL2_SRC_CORE_EVENT_QUEUE_HPP
