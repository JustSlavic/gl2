#include <iostream>
#include <application.h>
#include <version.h>
#include <logging/logging.h>

#include <deque>
#include <thread>
#include <mutex>
#include <condition_variable>


namespace EE {
    enum class EventType {
        mouse_event,
        keyboard_event,
    };

    struct IEvent {
        bool handled = false;

        virtual std::string get_name() = 0;
        virtual EventType get_type() = 0;
    };

#define DECLARE_EVENT_TYPE(TYPE) \
    static EventType get_static_type() { return TYPE; } \
    EventType get_type() override { return get_static_type(); }

    struct EventClick : public IEvent {
        int x = 0;
        int y = 0;

        EventClick(int x, int y) : x(x), y(y) {}

        std::string get_name() override { return "EventClick"; }
        DECLARE_EVENT_TYPE(EventType::keyboard_event);
    };

    struct EventMouseMove : public IEvent {
        std::string get_name() override { return "EventClick"; }
        DECLARE_EVENT_TYPE(EventType::mouse_event);
    };

    struct EventQueue {
        std::deque<IEvent*> q;

        std::mutex m_mutex;
        std::condition_variable cv;

        void push_event(IEvent* e) {
            std::lock_guard lock(m_mutex);

            q.push_back(e);
            cv.notify_one();
        }

        IEvent* get_event() {
            IEvent* result = nullptr;

            {
                std::unique_lock lock(m_mutex);
                cv.wait(lock, [this]() { return !this->q.empty(); });

                result = q.front();
            }

            return result;
        }

        void pop_event() {
            if (q.empty()) { return; }

            q.pop_front();
        }
    };

    class Dispatcher {
        template<typename EventType, class C, bool(C::* M)(EventType*)>
        static bool invoke(void* instance, IEvent* e) {
            if (EventType::get_static_type() != e->get_type()) return false;
            return (static_cast<C*>(instance)->*M)(static_cast<EventType*>(e));
        }

    public:
        template<typename EventType, class C, bool(C::* M)(EventType*)>
        void bind(C* instance) {
            auto pair = std::make_pair(&invoke<EventType, C, M>, instance);
            targets.push_back(pair);
        }

        bool operator()(IEvent* e) {
            for (auto pair : targets) {
                if ((pair.first)(pair.second, e)) {
                    return true;
                }
            }

            return false;
        }

    private:
        using Fn = bool(*)(void*, IEvent*);
        std::vector<std::pair<Fn, void*>> targets;
    };

    struct EventReceiver {
        Dispatcher d;

        template <typename EventType, typename Host, bool(Host::*M)(EventType*)>
        void bind(Host* host) {
            d.bind<EventType, Host, M>(host);
        }

        bool handle(IEvent* e) {
            return d(e);
        }
    };

    struct UiLayer : public EventReceiver {
        UiLayer() {
            bind<EventClick, UiLayer, &UiLayer::on_click>(this);
            bind<EventMouseMove, UiLayer, &UiLayer::on_mouse_move>(this);
        }

        bool on_click(EventClick* e) {
            if (e->x > 0) {
                printf("On click event handled! (%d, %d)\n", e->x, e->y);
                return true;
            }
            return false;
        }

        bool on_mouse_move(EventMouseMove* e) {
            printf("Move move event handled!\n");
            return true;
        }
    };

    struct WorldLayer : public EventReceiver {
        WorldLayer() {
            bind<EventClick, WorldLayer, &WorldLayer::on_click>(this);
        }

        bool on_click(EventClick* e) {
            printf("Character shoots: pew pew!!!\n");
            return true;
        }
    };
}


int main(int argc, char** argv, char** env) {
    {
        EE::EventQueue queue;
        queue.push_event(new EE::EventClick(100, 100));
        queue.push_event(new EE::EventMouseMove());
        queue.push_event(new EE::EventClick(-10, 100));
        queue.push_event(new EE::EventMouseMove());
        queue.push_event(new EE::EventMouseMove());
        queue.push_event(new EE::EventMouseMove());
        queue.push_event(new EE::EventMouseMove());
        queue.push_event(new EE::EventClick(20, 20));

        std::vector<EE::EventReceiver*> layer_stack;
        layer_stack.push_back(new EE::UiLayer());
        layer_stack.push_back(new EE::WorldLayer());

        EE::Dispatcher d;

        while (!queue.q.empty()) {
            auto pEvent = queue.get_event();
            for (auto& layer : layer_stack) {
                bool handled = layer->handle(pEvent);
                if (handled) break;
            }
            queue.pop_event();
        }
    }

    // return 0;


    LogGlobalContext::instance()
        .set_level(Log::Level::Info)
        .attach(std::cout);
        // .attach(std::string("log/") + now_as_string() + ".log");

    LOG_INFO << "Welcome to Gir2 v." << version;

    gl2::Application_2 app;

    if (app.initialize()) {
        LOG_ERROR << "Failed to initialize some context!";
        std::exit(1);
    }

    return app.run();
}
