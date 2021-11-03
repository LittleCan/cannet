#include "Thread.h"
#include "CurrentThread.h"

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

    struct ThreadData {
        typedef Thread::ThreadFunc threadFunc;
        threadFunc func_;
        std::string name_;
        pid_t *tid_;
        CoutDownLatch *latch_;

        ThreadData(const threadFunc &func, const std::string &name, pid_t *tid, CoutDownLatch *latch) :
                func_(func), name_(name), tid_(tid), latch_(latch) {};

        void runInThread() {
            *tid_ = tid();
            tid_ = nullptr;
            latch_->countDown();
            latch_ = nullptr;
            t_threadName = name_.empty() ? "Thread" : name_.c_str();
            prctl(PR_SET_NAME, t_threadName);
            func_();
            t_threadName = "finished";
        }
    };

    void *startThread(void *obj) {
        ThreadData *data = static_cast<ThreadData *>(obj);
        data->runInThread();
        delete data;
        return nullptr;
    }

    Thread::Thread(const ThreadFunc &func, const std::string &n) :
            started_(false), joined_(false), pthreadId_(0), tid_(0), func_(func), name_(n), latch_(1) {
        setDefaultName();
    }

    Thread::~Thread() {
        if (started_ && !joined_) pthread_detach(pthreadId_);
    }

    void Thread::setDefaultName() {
        if (name_.empty()) {
            char buf[32];
            snprintf(buf, sizeof(buf), "Thread");
            name_ = buf;
        }
    }

    void Thread::start() {
        assert(!started_);
        started_ = true;
        ThreadData *data = new ThreadData(func_, name_, &tid_, &latch_);
        if (pthread_create(&pthreadId_, NULL, &startThread, data)) {
            started_ = false;
            delete data;
        } else {
            latch_.wait();
            assert(tid_ > 0);
        }
    }

    int Thread::join() {
        assert(started_);
        assert(!joined_);
        joined_ = true;
        return pthread_join(pthreadId_, NULL);
    }


}