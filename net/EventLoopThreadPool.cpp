#include <functional>
#include <cassert>
#include "EventLoop.h"
#include "EventLoopThread.h"
#include "EventLoopThreadPool.h"

namespace cannet
{

    EventLoopThreadPool::EventLoopThreadPool(EventLoop *baseLoop)
        : baseLoop_(baseLoop),
          started_(false),
          numThreads_(0),
          next_(0)
    {
    }

    EventLoopThreadPool::~EventLoopThreadPool() = default;

    void EventLoopThreadPool::start(const ThreadInitCallback &cb)
    {
        assert(!started_);
        baseLoop_->assertInLoopThread();
        started_ = true;

        for (size_t i = 0; i < numThreads_; ++i)
        {
            auto *t = new EventLoopThread(cb);
            threads_.emplace_back(std::unique_ptr<EventLoopThread>(t));
            loops_.push_back(t->startLoop()); //启动eventloopthreads线程，在进入事件循环之前，会调用cb
        }
    }

    EventLoop *EventLoopThreadPool::getNextLoop()
    {
        baseLoop_->assertInLoopThread();
        assert(started_);
        EventLoop *loop = baseLoop_;
        //如果loops为空，则loop指向baseloop_
        //如果不为空，按照round-robin轮叫的调度方式选择一个eventloop

        if (!loops_.empty())
        {
            // round-robin
            loop = loops_[next_];
            next_ = (next_ + 1) % numThreads_;
        }
        return loop;
    }
}