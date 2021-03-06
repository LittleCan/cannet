#ifndef CANNET_NET_EVENTLOOP_H
#define CANNET_NET_EVENTLOOP_H

#include <vector>
#include <memory>
#include <functional>
#include <atomic>
#include "../base/Noncopyable.h"
#include <mutex>
#include <chrono>
#include "../base/CurrentThread.h"
#include "../base/Thread.h"
#include "CallBacks.h"
#include "../base/Logger.h"
#include "Timer.h"
#include "TimerManager.h"

namespace cannet
{
    class Channel;
    class Epoll;
    class TimerManager;
    class Timer;
    class EventLoop : noncopyable
    {
    public:
        using Functor = std::function<void()>;
        explicit EventLoop();
        ~EventLoop();
        void loop();
        void quit();

        void runInLoop(const Functor &cb);
        void queueInLoop(const Functor &cb);
        std::weak_ptr<Timer> runAt(const std::chrono::steady_clock::time_point &time, TimerCallback cb);
        std::weak_ptr<Timer> runEvery(const std::chrono::nanoseconds &interval, TimerCallback cb);
        std::weak_ptr<Timer> runAfter(const std::chrono::nanoseconds &delay, TimerCallback cb);

        void wakeup() const;
        void updateChannel(Channel *channel); //在poller中添加或者更新通道
        void removeChannel(Channel *channel); //在poller中移除通道

        void assertInLoopThread()
        {
            if (!isInLoopThread())
            {
                abortNotInLoopThread();
            }
        }

        bool isInLoopThread() const { return threadId_ == CurrentThread::tid(); }
        bool eventHandling() const { return eventHandling_; }
        void createConnection(int sockFd,
                              const ConnectionCallback &connectCallback,
                              const MessageCallback &messageCallback,
                              const WriteCompleteCallback &writeCompleteCallback);

    private:
        void abortNotInLoopThread();
        void handleRead() const;
        void runFunctors(); //执行转交给I\O的任务

        using ChannelVec = std::vector<Channel *>; //事件分发器列表
        bool looping_;
        std::atomic<bool> quit_;
        bool eventHandling_;
        bool callingFunctors_;
        const pid_t threadId_;

        std::unique_ptr<Epoll> epoll_;
        std::unique_ptr<TimerManager> timerManger_;
        int wakeupFd_;
        std::unique_ptr<Channel> wakeupChannel_;

        ChannelVec activeChannels_;

        mutable std::mutex mutex_;
        std::vector<Functor> functors_;
    };

}

#endif