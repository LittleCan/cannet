#include "CountDownLatch.h"

namespace cannet {

    void CoutDownLatch::wait() {
        std::unique_lock<std::mutex> lock_(mutex_);
        while (count_ > 0) {
            cond_.wait(lock_);
        }
    }

    void CoutDownLatch::countDown() {
        std::unique_lock<std::mutex> lock_(mutex_);
        --count_;
        if(count_ == 0){
            cond_.notify_all();
        }
    }
}