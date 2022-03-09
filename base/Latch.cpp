#include "Latch.h"

namespace cannet
{
    Latch::Latch(int count) : count_(count), mutex_(), condition_()
    {
    }

    void Latch::wait()
    {
        std::unique_lock<std::mutex> lock(mutex_);
        while (count_ > 0)
        {
            condition_.wait(lock);
        }
    }
    void Latch::countDown()
    {
        std::unique_lock<std::mutex> lock(mutex_);
        --count_;
        if (count_ == 0)
        {
            condition_.notify_all();
        }
    }
    int Latch::getCount() const
    {
        std::unique_lock<std::mutex> lock(mutex_);
        return count_;
    }
}