#ifndef CANNET_POLLER_H
#define CANNET_POLLER_H

#include <cassert>
#include <poll.h>
#include <ctime>
#include <sys/types.h>
#include <cstring>
#include <atomic>
#include <map>
#include "Noncopyable.h"

namespace cannet {
    const int kMaxEvents = 2000;
    const int kReadEvent = POLLIN;
    const int kWriteEvent = POLLOUT;

    class PollerBase : private noncopyable {
    public:
        int64_t id_;
        int lastActive_;

        PollerBase() : lastActive_(-1) {
            std::atomic<int64_t> id(0);
            id_ = ++id;
        }

        virtual void addChannel(Channel *ch) = 0;
    };
}

#endif //CANNET_POLLER_H
