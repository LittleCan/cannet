#ifndef CANNET_THREAD_H
#define CANNET_THREAD_H

#include <thread>
#include <sys/syscall.h>
#include <sys/prctl.h>
#include <sys/types.h>
#include <unistd.h>
#include <memory>
#include <string>
#include <functional>
#include <cstdio>
#include <pthread.h>
#include "Noncopyable.h"
#include "CountDownLatch.h"
#include "cassert"
#include "pthread.h"

namespace cannet {
    class Thread : private noncopyable {
    public:
        typedef std::function<void()> ThreadFunc;

        explicit Thread(const ThreadFunc &func, const std::string &n);

        ~Thread();

        void start();

        int join();

        bool started() const {
            return started_;
        }

        pid_t tid() const {
            return tid_;
        }

        const std::string &name() const {
            return name_;
        }

    private:
        void setDefaultName();

        bool started_;
        bool joined_;
        pthread_t pthreadId_;
        pid_t tid_;
        ThreadFunc func_;
        std::string name_;
        CoutDownLatch latch_;
    };
}

#endif //CANNET_THREAD_H
