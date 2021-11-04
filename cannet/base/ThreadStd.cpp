#include "ThreadStd.h"

namespace cannet {
    __thread int t_cachedTid = 0;
    __thread char t_tidString[32];
    __thread int t_tidStringLength = 6;
    __thread const char *t_threadName = "default";

    ThreadStd::ThreadStd(const ThreadStd::ThreadFunc &func) :
            started_(false), pthreadId_(0), tid_(0), func_(func) {
    }

    ThreadStd::~ThreadStd() {
        if (started_ && thread_.joinable()) {
            thread_.detach();
        }
    }

    void ThreadStd::start() {
        assert(!started_);
        started_ = true;
        try {
            thread_ = std::thread(func_);
            started_ = true;
        } catch (...) {
            return;
        }
    }

    void ThreadStd::join() {
        if (started_ && thread_.joinable()) {
            thread_.join();
        }
    }
}
