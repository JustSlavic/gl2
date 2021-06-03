#ifndef GL2_SRC_CORE_EVENT_QUEUE_HPP
#define GL2_SRC_CORE_EVENT_QUEUE_HPP


#include "events.hpp"
#include <deque>
#include <mutex>
#include <condition_variable>
#include <memory>


namespace core {


struct EventQueue {
    std::deque<std::shared_ptr<IEvent>> queue;
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

        q.queue.push_back(std::make_shared<EventType>(std::forward<Args...>(args...)));
        q.cv.notify_one();
    }


    template <typename EventType>
    static void push_event() {
        auto& q = instance();
        std::lock_guard lock(q.m_mutex);

        q.queue.push_back(std::make_shared<EventType>());
        q.cv.notify_one();
    }


    static std::shared_ptr<IEvent> get_event() {
        auto& queue = instance();
        std::shared_ptr<IEvent> result = nullptr;

        {
            std::unique_lock lock(queue.m_mutex);
            queue.cv.wait(lock, [&queue]() { return !queue.queue.empty(); });

            result = queue.queue.front();
        }

        return result;
    }

    static void pop_event() {
        auto& queue = instance();
        if (queue.empty()) { return; }

        queue.queue.pop_front();
    }

private:
    EventQueue() = default;
    EventQueue(const EventQueue&) = delete;
    EventQueue(EventQueue&&) = delete;
};


} // core



#endif // GL2_SRC_CORE_EVENT_QUEUE_HPP
