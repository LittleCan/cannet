#ifndef CANNET_NET_EVENTLOOPTHREAD_H
#define CANNET_NET_EVENTLOOPTHREAD_H
#include "base/Noncopyable.h"
#include <mutex>
#include <condition_variable>
#include "base/Thread.h"
namespace cannet
{
    class EventLoop;

    class EventLoopThread : noncopyable
    {
    public:
        using ThreadInitCallback = std::function<void(EventLoop *)>;

        explicit EventLoopThread(ThreadInitCallback cb = ThreadInitCallback());
        ~EventLoopThread();
        EventLoop *startLoop();

    private:
        void threadFunc(); //传到线程里面的函数

        EventLoop *loop_; // loop_指针指向一个eventloop对象
        bool exiting_;    //是否退出
        Thread thread_;
        std::mutex mutex_;
        std::condition_variable cond_;
        ThreadInitCallback callback_;
    };
}

#endif