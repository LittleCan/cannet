#ifndef CANNET_COUNTDOWNLATCH_H
#define CANNET_COUNTDOWNLATCH_H

#include <mutex>
#include <condition_variable>
#include "Noncopyable.h"

namespace cannet {
    class CoutDownLatch : noncopyable {
    public:
        explicit CoutDownLatch(int count) : count_(count) {};

        ~CoutDownLatch() = default;

        void wait();

        void countDown();

    private:
        mutable std::mutex mutex_;
        std::condition_variable cond_;
        int count_;
    };
}

#endif //CANNET_COUNTDOWNLATCH_H
