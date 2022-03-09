#ifndef CANNET_BASE_THREAD_H
#define CANNET_BASE_THREAD_H
#include <thread>
#include <string>
#include <functional>
#include <atomic>
#include "Noncopyable.h"
#include "Latch.h"

namespace cannet
{
    class Thread : noncopyable
    {
    public:
        using ThreadFunc = std::function<void()>;
        explicit Thread(ThreadFunc);
        ~Thread();

        void start();
        bool started() const
        {
            return started_;
        }

    private:
        bool started_;
        std::unique_ptr<std::thread> thread_;
        pid_t tid_;
        ThreadFunc func_;
        Latch latch_;
    };
}

#endif