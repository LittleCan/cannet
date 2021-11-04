#ifndef CANNET_THREADSTD_H
#define CANNET_THREADSTD_H

#include <mutex>
#include <thread>
#include <condition_variable>
#include <functional>
#include <cassert>

namespace cannet {

    class ThreadStd {
    public:
        typedef std::function<void()> ThreadFunc;

        explicit ThreadStd(const ThreadFunc &func);

        ~ThreadStd();

        void start();

        void join();

        bool started() const {
            return started_;
        }

        pid_t tid() const {
            return tid_;
        }


    private:

        bool started_;
        pthread_t pthreadId_;
        pid_t tid_;
        ThreadFunc func_;
        std::thread thread_;
    };
}

#endif //CANNET_THREADSTD_H
