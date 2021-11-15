#include "ThreadStd.h"

namespace cannet {
    __thread int t_cachedTid = 0;
    __thread char t_tidString[32];
    __thread int t_tidStringLength = 6;
    __thread const char *t_threadName = "default";

    pid_t getTid() {
        return static_cast<pid_t>(::syscall(SYS_gettid));
    }

    void cacheTid() {
        if (t_cachedTid == 0) {
            t_cachedTid = getTid();
            t_tidStringLength = snprintf(t_tidString, sizeof(t_tidString), "%5d", t_cachedTid);
        }
    }

    int tid() {
        if (__builtin_expect(t_cachedTid == 0, 0)) {
            cacheTid();
        }
        return t_cachedTid;
    }

    const char *tidString()  // for logging
    {
        return t_tidString;
    }

    int tidStringLength()  // for logging
    {
        return t_tidStringLength;
    }

    const char *name() { return t_threadName; }


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
