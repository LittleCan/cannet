#ifndef CANNET_BASE_LATCH
#define CANNET_BASE_LATCH

#include <condition_variable>
#include <mutex>
#include "Noncopyable.h"

namespace cannet
{
    class Latch : noncopyable
    {
    public:
        explicit Latch(int count);
        void wait();
        void countDown();
        int getCount() const;

    private:
        mutable std::mutex mutex_;
        std::condition_variable condition_;
        int count_;
    };
}

#endif