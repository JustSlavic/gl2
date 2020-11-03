#include "watcher.h"
#include <logging/logging.h>

#ifdef __linux__
#include <unistd.h>
#include <sys/stat.h>
#else
#error "This platform not supported"
#endif


EventFileChanged::EventFileChanged(std::string s)
    : filename(std::move(s))
{
}

Watcher::Watcher(std::string filename)
    : filename(std::move(filename))
{
    struct stat status;
    int e = stat(this->filename.c_str(), &status);
    if (e) {
        LOG_ERROR << "Cannot get status of " << this->filename << " file";
        std::exit(1);
    }

    seconds = status.st_mtim.tv_sec;
    nanoseconds = status.st_mtim.tv_nsec;

    Dispatcher<EventFrameFinished>::subscribe(EVENT_CALLBACK(on_frame_finished));
}

void Watcher::on_frame_finished(EventFrameFinished) {
    check();
}

void Watcher::check() {
    struct stat status;
    int e = stat(filename.c_str(), &status);
    if (e) {
        LOG_ERROR << "Cannot get status of " << filename << " file";
        std::exit(1);
    }

    if (seconds < status.st_mtim.tv_sec ||
        (seconds == status.st_mtim.tv_sec && nanoseconds < status.st_mtim.tv_nsec)) 
    {
        seconds = status.st_mtim.tv_sec;
        nanoseconds = status.st_mtim.tv_nsec;
        emit(EventFileChanged(filename));
    }
}
