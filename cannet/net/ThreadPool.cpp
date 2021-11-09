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
}