#include "Thread.h"
#include <string>
#include "CurrentThread.h"
#include <exception>
#include <cassert>

namespace cannet
{
    namespace CurrentThread
    {
        thread_local int t_cachedTid = 0;
        thread_local std::string t_tidString;
    }

    namespace detail
    {
        using ThreadFunc = Thread::ThreadFunc;

        void runInThread(const ThreadFunc &func, pid_t *tid, Latch *latch)
        {
            *tid = CurrentThread::tid();
            latch->countDown();
            try
            {
                func();
            }
            catch (const std::exception &ex)
            {
                fprintf(stderr, "exception caught in Thread %d\n", CurrentThread::tid());
                fprintf(stderr, "reason: %s\n", ex.what());
                abort();
            }
            catch (...)
            {
                fprintf(stderr, "unknown exception caught in Thread %d\n", CurrentThread::tid());
                throw; // rethrow
            }
        }
    }

    bool CurrentThread::isMainThread()
    {
        return CurrentThread::tid() == getpid();
    }

    Thread::Thread(ThreadFunc func) : started_(false), tid_(0), func_(std::move(func)), latch_(1)
    {
    }

    Thread::~Thread()
    {
        if (started_ && thread_->joinable())
            thread_->join();
    }

    void Thread::start()
    {
        assert(!started_);
        started_ = true;
        thread_ = std::make_unique<std::thread>(std::thread(detail::runInThread, func_, &tid_, &latch_));
        if (thread_ == nullptr)
        {
            started_ = false;
        }
        else
        {
            latch_.wait();
            assert(tid_ > 0);
        }
    }
}