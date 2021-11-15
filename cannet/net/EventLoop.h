#ifndef CANNET_EVENTLOOP_H
#define CANNET_EVENTLOOP_H

#include <functional>
#include <memory>
#include <vector>
#include "Channel.h"
#include "Epoll.h"
#include "Util.h"
#include "ThreadStd.h"
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <iostream>
#include "Util.h"
#include "Logger.h"
#include <mutex>
#include <iostream>

namespace cannet {
    class Channel;

    class Epoll;

    class EventLoop {
    public:
        typedef std::function<void()> Functor;

        EventLoop();

        ~EventLoop();

        void loop();

        void quit();

        void runInLoop(Functor &&cb);

        void queueInLoop(Functor &&cb);

        bool isInLoopThread() const { return threadId_ == tid(); }

        void assertInLoopThread() { assert(isInLoopThread()); }

        void shutdown(std::shared_ptr<Channel> channel);

        void removeFromPoller(std::shared_ptr<Channel> channel);

        void updatePoller(std::shared_ptr<Channel> channel, int timeout = 0);

        void addToPoller(std::shared_ptr<Channel> channel, int timeout = 0);

    private:
        // 声明顺序 wakeupFd_ > pwakeupChannel_
        bool looping_;
        std::shared_ptr<Epoll> poller_;
        int wakeupFd_;
        bool quit_;
        bool eventHandling_;
        mutable std::mutex mutex_;
        std::vector<Functor> pendingFunctors_;
        bool callingPendingFunctors_;
        const pid_t threadId_;
        std::shared_ptr<Channel> pwakeupChannel_;

        void wakeup();

        void handleRead();

        void doPendingFunctors();

        void handleConn();
    };

}

#endif //CANNET_EVENTLOOP_H
