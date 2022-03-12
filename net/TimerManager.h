#ifndef CANNET_NET_TIMERMANAGER_H
#define CANNET_NET_TIMERMANAGER_H
#include "../base/Noncopyable.h"
#include <queue>
#include <vector>
#include <memory>
#include "Timer.h"
#include "Channel.h"


namespace cannet
{

    class EventLoop;
    struct TimerCmp
    {
        bool operator()(const std::shared_ptr<Timer> &a, const std::shared_ptr<Timer> &b)
        {
            if (a->getExpiration() == b->getExpiration())
            {
                return a->getSeq() > b->getSeq();
            }
            else
            {
                return a->getExpiration() > b->getExpiration();
            }
        }
    };

    class TimerManager : noncopyable
    {
    public:
        using TimerQueue = std::priority_queue<std::shared_ptr<Timer>, std::vector<std::shared_ptr<Timer>>, TimerCmp>;
        explicit TimerManager(EventLoop *loop);
        ~TimerManager();

        std::weak_ptr<Timer> addTimer(TimerCallback cb,
                                      Timer::TimePoint when,
                                      Timer::TimeUnit interval);
        void cancel(const std::weak_ptr<Timer> &timeId);

    private:
        void addTimerInLoop(const std::shared_ptr<Timer> &timer);
        static void cancelInLoop(const std::weak_ptr<Timer> &timeId);

        void handleRead();
        std::vector<std::shared_ptr<Timer>> getExpired(const Timer::TimePoint &now);
        void reset(const std::vector<std::shared_ptr<Timer>> &expired, const Timer::TimePoint &now);

        bool insert(const std::shared_ptr<Timer> &timer);

        EventLoop *loop_;
        const int timerFd_;
        Channel timerFdChannel_;
        TimerQueue timers_;
    };

}

#endif