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
#include "EventBase.h"
#include "Conn.h"
#include "Util.h"
#include <fcntl.h>
#include <sys/epoll.h>

namespace cannet {

    class Channel;

    const int kMaxEvents = 2000;
    const int kReadEvent = POLLIN;
    const int kWriteEvent = POLLOUT;

    class PollerBase : private noncopyable {
    public:
        int64_t id_;
        int lastActive_;

        PollerBase() : lastActive_(-1) {
            static std::atomic<int64_t> id(0);
            id_ = ++id;
        };

        virtual void addChannel(Channel *ch) = 0;

        virtual void removeChannel(Channel *ch) = 0;

        virtual void updateChannel(Channel *ch) = 0;

        virtual void loop_once(int waitMs) = 0;

        virtual ~PollerBase() {};
    };

    PollerBase *createPoller();
}

#endif //CANNET_POLLER_H
