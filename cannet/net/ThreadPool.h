#ifndef CANNET_THREADPOOL_H
#define CANNET_THREADPOOL_H

#include <atomic>
#include <condition_variable>
#include <functional>
#include <limits>
#include <list>
#include <mutex>
#include <thread>
#include <vector>
#include <cassert>
#include <memory>
#include "Util.h"
#include "utility"
#include "cassert"

namespace cannet {

    template<typename T>
    class SafeQueue : private std::mutex, private noncopyable {
    private:
        std::list<T> items_;
        std::condition_variable ready_;
        size_t capacity_;
        std::atomic<bool> exit_;

        void wait_ready(std::unique_lock<std::mutex> &lk, int waitMs);

    public:
        static const int wait_infinite = std::numeric_limits<int>::max();

        SafeQueue(size_t capacity = 0) : capacity_(capacity), exit_(false) {};

        bool push(T &&v);

        T pop_wait(int waitMs = wait_infinite);

        bool pop_wait(T *v, int waitMs = wait_infinite);

        size_t size();

        void exit();

        bool exited();
    };


    typedef std::function<void()> Task;


    class ThreadPool : private noncopyable {
    private:
        SafeQueue<Task> tasks_;
        std::vector<std::thread> threads_;
    public:
        ThreadPool(int threads, int taskCapacity = 0, bool start = true);

        ~ThreadPool();

        void start();

        ThreadPool &exit() {
            tasks_.exit();
            return *this;
        }

        void join();

        bool addTask(Task &&task);

        bool addTask(Task &task) {
            return addTask(Task(task));
        }

        size_t taskSize() {
            return tasks_.size();
        }
    };
}


#endif //CANNET_THREADPOOL_H
