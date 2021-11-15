#ifndef CANNET_EVENTLOOPTHREADPOOL_H
#define CANNET_EVENTLOOPTHREADPOOL_H

#include <memory>
#include <vector>
#include "EventLoopThread.h"
#include "Logger.h"
#include "Noncopyable.h"


namespace cannet {
    class EventLoopThreadPool : noncopyable {
    public:
        EventLoopThreadPool(EventLoop *baseLoop, int numThreads);

        ~EventLoopThreadPool() { LOG << "~EventLoopThreadPool()"; }

        void start();

        EventLoop *getNextLoop();

    private:
        EventLoop *baseLoop_;
        bool started_;
        int numThreads_;
        int next_;
        std::vector<std::shared_ptr<EventLoopThread>> threads_;
        std::vector<EventLoop *> loops_;
    };

}


#endif //CANNET_EVENTLOOPTHREADPOOL_H
