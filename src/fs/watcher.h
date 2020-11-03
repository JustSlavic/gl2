#pragma once

#include <es/event_system.h>


struct EventFileChanged {
    std::string filename;

    EventFileChanged(std::string s);
};

struct Watcher : public IEmitter {
    std::string filename;
    i64 seconds = 0;  // from epoch
    i64 nanoseconds = 0;

    Watcher(std::string filename);

    void check();
    void on_frame_finished(EventFrameFinished);
};
