#ifndef CANNET_NET_TIMER_H
#define CANNET_NET_TIMER_H

#include "../base/Noncopyable.h"
#include <chrono>
#include <utility>
#include "CallBacks.h"

namespace cannet
{
    class Timer : noncopyable
    {
    public:
        using TimeUnit = std::chrono::nanoseconds;
        using TimePoint = std::chrono::steady_clock::time_point;
        Timer(TimerCallback cb, TimePoint when, TimeUnit interval)
            : callback_(std::move(cb)),
              expiration_(when),
              interval_(interval),
              repeat_(interval_.count() > 0),
              Delete_(false),
              seq_(sTimerNum_++)
        {
        }
        const TimePoint &getExpiration() const { return expiration_; }
        int64_t getSeq() const { return seq_; }
        bool isRepeat() const { return repeat_; }
        bool isDelete() const { return Delete_; }
        void setDelete(bool on) { Delete_ = on; }
        void restart(const TimePoint &now) { expiration_ = now + interval_; }
        void run() const { callback_(); }

    private:
        const TimerCallback callback_;
        TimePoint expiration_;
        TimeUnit interval_; 
        const bool repeat_;
        bool Delete_;
        const int64_t seq_;
        static int64_t sTimerNum_;
    };

}

#endif
