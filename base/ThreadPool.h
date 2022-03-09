#ifndef CANNET_BASE_THREADPOOL_H
#define CANNET_BASE_THREADPOOL_H

#include "Noncopyable.h"
#include <memory>
#include <utility>
#include <functional>
#include "Thread.h"
#include "ThreadSafeQueue.h"
#include <vector>

namespace cannet
{
    class ThreadPool : noncopyable
    {
    public:
        using Task = std::function<void()>;
        explicit ThreadPool();
        ~ThreadPool();
        void setThreadInitCallback(Task cb)
        {
            threadInitCallback_ = std::move(cb);
        }
        void start(unsigned numThreads);
        void stop();
        void addTask(Task task)
        {
            taskQueue_.push(std::move(task));
        }

    private:
        void runInThread();
        Task threadInitCallback_;
        bool running_;
        std::vector<std::unique_ptr<Thread>> threads_;
        ThreadSafeQueue<Task> taskQueue_;
    };
}

#endif