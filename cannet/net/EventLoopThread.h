#ifndef CANNET_EVENTLOOPTHREAD_H
#define CANNET_EVENTLOOPTHREAD_H

#include "EventLoop.h"
#include <condition_variable>
#include <mutex>
#include "ThreadStd.h"
#include "Noncopyable.h"


namespace cannet {
    class EventLoopThread : noncopyable {
    public:
        EventLoopThread();

        ~EventLoopThread();

        EventLoop *startLoop();

    private:
        void threadFunc();

        EventLoop *loop_;
        bool exiting_;
        ThreadStd thread_;
        std::mutex mutex_;
        std::condition_variable cond_;
    };

}

#endif //CANNET_EVENTLOOPTHREAD_H
