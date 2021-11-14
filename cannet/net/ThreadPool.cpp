#include "ThreadPool.h"

namespace cannet {

    ThreadPool::ThreadPool(int threads, int taskCapacity, bool start) : tasks_(taskCapacity), threads_(threads) {
        if (start) {
            this->start();
        }
    }

    ThreadPool::~ThreadPool() {
        assert(tasks_.exited());
        if (tasks_.size()) {
            fprintf(stderr, "%lu tasks no processed when thread pool exited\n", tasks_.size());
        }
    }

    void ThreadPool::join() {
        for (auto &t: threads_) {
            t.join();
        }
    }

    bool ThreadPool::addTask(Task &&task) {
        return tasks_.push(std::move(task));
    }

    void ThreadPool::start() {
        for (auto &th: threads_) {
            std::thread t([this] {
                while (!tasks_.exited()) {
                    Task task;
                    if (tasks_.pop_wait(&task)) {
                        task();
                    }
                }
            });
            th.swap(t);
        }
    }


    template<typename T>
    void SafeQueue<T>::wait_ready(std::unique_lock<std::mutex> &lk, int waitMs) {
        if (exit_ || !items_.empty()) {
            return;
        }
        if (waitMs == wait_infinite) {
            ready_.wait(lk, [this] { return exit_ || !items_.empty(); });
        } else if (waitMs > 0) {
            auto tp = std::chrono::steady_clock::now() + std::chrono::microseconds(waitMs);
            while (ready_.template wait_until(lk, tp) != std::cv_status::timeout && items_.empty() && !exit_) {

            }
        }
    }

    template<typename T>
    bool SafeQueue<T>::push(T &&v) {
        std::lock_guard<std::mutex> lk(*this);
        if (exit_ || (capacity_ && items_.size() >= capacity_)) {
            return false;
        }
        items_.push_back(std::move(v));
        ready_.notify_one();
        return true;
    }

    template<typename T>
    T SafeQueue<T>::pop_wait(int waitMs) {
        std::unique_lock<std::mutex> lk(*this);
        wait_ready(lk, waitMs);
        if (items_.empty()) {
            return T();
        }
        T r = std::move(items_.front());
        items_.pop_front();
        return r;
    }

    template<typename T>
    bool SafeQueue<T>::pop_wait(T *v, int waitMs) {
        std::unique_lock<std::mutex> lk(*this);
        wait_ready(lk, waitMs);
        if (items_.empty()) {
            return false;
        }
        *v = std::move(items_.front());
        items_.pop_front();
        return true;
    }

    template<typename T>
    size_t SafeQueue<T>::size() {
        std::lock_guard<std::mutex> lk(*this);
        return items_.size();
    }

    template<typename T>
    void SafeQueue<T>::exit() {
        exit_ = true;
        std::lock_guard<std::mutex> lk(*this);
        ready_.notify_all();
    }

    template<typename T>
    bool SafeQueue<T>::exited() {
        return exit_;
    };
}