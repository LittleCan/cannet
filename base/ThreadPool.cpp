#include "ThreadPool.h"

namespace cannet
{
    ThreadPool::ThreadPool() : threadInitCallback_(), running_(false), taskQueue_(){

                                                                       };

    ThreadPool::~ThreadPool()
    {
        if (running_)
        {
            stop();
        }
    }

    void ThreadPool::stop()
    {
        running_ = false;
        //error : notify_all not work
        taskQueue_.notify_all();
    }

    void ThreadPool::start(unsigned numThreads)
    {
        running_ = true;
        threads_.reserve(static_cast<unsigned long>(numThreads));
        for (unsigned i = 0; i < numThreads; i++)
        {
            threads_.emplace_back(std::make_unique<Thread>([this]
                                                           { runInThread(); }));
            threads_[i]->start();
        }
        if (numThreads == 0 && threadInitCallback_)
        {
            threadInitCallback_();
        }
    }

    void ThreadPool::runInThread()
    {
        if (threadInitCallback_)
        {
            threadInitCallback_();
        }
        while (running_)
        {
            Task task;
            taskQueue_.waitAndPop(task);
            if (task)
            {
                task();
            }
        }
    }
}