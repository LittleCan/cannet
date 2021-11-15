#ifndef CANNET_THREADSTD_H
#define CANNET_THREADSTD_H

#include <mutex>
#include <thread>
#include <condition_variable>
#include <functional>
#include <cassert>
#include <sys/syscall.h>
#include <sys/prctl.h>
#include <sys/types.h>
#include <unistd.h>

namespace cannet {

    pid_t getTid();

    void cacheTid();

    int tid();

    const char *tidString();

    int tidStringLength();

    const char *name();

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
