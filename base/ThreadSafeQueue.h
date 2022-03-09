#ifndef CANNET_BASE_THREADSAFEQUEUE_H
#define CANNET_BASE_THREADSAFEQUEUE_H

#include "Noncopyable.h"
#include <memory>
#include <mutex>
#include <condition_variable>

namespace cannet
{
    template <typename T>
    class ThreadSafeQueue : noncopyable
    {
    public:
        ThreadSafeQueue() : head_(new node), tail_(head_.get()){};

        void push(T x)
        {
            std::shared_ptr<T> newVal(std::make_shared<T>(std::move(x)));
            std::unique_ptr<node> p(new node);
            {
                std::lock_guard<std::mutex> lock(tailMutex_);
                tail_->val_ = newVal;
                node *const newTail = p.get();
                tail_->next_ = std::move(p);
                tail_ = newTail;
            }
            cv_.notify_one();
        }

        void notify_all(){
            std::lock_guard<std::mutex> lock(headMutex_);
            // error:pop will check empty
            if(empty())
                cv_.notify_all();
        }

        std::shared_ptr<T> waitAndPop()
        {
            std::unique_ptr<node> oldHead = waitPopHead();
            return oldHead->val_;
        }

        void waitAndPop(T &x)
        {
            std::unique_ptr<node> oldHead = waitPopHead(x);
        }

        std::shared_ptr<T> tryPop()
        {
            std::unique_ptr<node> oldHead = tryPopHead();
            return oldHead ? oldHead->val_ : std::shared_ptr<T>();
        }

        bool tryPop(T &x)
        {
            std::unique_ptr<node> oldHead = tryPopHead(x);
            return oldHead;
        }

        bool empty() 
        {
            std::lock_guard<std::mutex> lock(headMutex_);
            return head_.get() == getTail();
        }

    private:
        struct node
        {
            std::shared_ptr<T> val_;
            std::unique_ptr<node> next_;
        };

        std::unique_ptr<node> head_;
        node *tail_;
        mutable std::mutex headMutex_;
        std::mutex tailMutex_;
        std::condition_variable cv_;

        node *getTail()
        {
            std::lock_guard<std::mutex> lock(tailMutex_);
            return tail_;
        }

        std::unique_ptr<node> popHead()
        {
            std::unique_ptr<node> oldHead = std::move(head_);
            head_ = std::move(oldHead->next_);
            return oldHead;
        }

        std::unique_lock<std::mutex> waitForData()
        {
            std::unique_lock<std::mutex> lock(headMutex_);
            cv_.wait(lock, [this]
                     { return head_.get() != getTail(); });
            return lock;
        }

        std::unique_ptr<node> waitPopHead()
        {
            std::unique_lock<std::mutex> lock(waitForData());
            return popHead();
        }

        std::unique_ptr<node> waitPopHead(T &x)
        {
            std::unique_lock<std::mutex> lock(waitForData());
            x = std::move(*head_->val_);
            return popHead();
        }

        std::unique_ptr<node> tryPopHead()
        {
            std::lock_guard<std::mutex> lock(headMutex_);
            if (head_.get() == getTail())
                return std::unique_ptr<node>();
            return popHead();
        }

        std::unique_ptr<node> tryPopHead(T &x)
        {
            std::lock_guard<std::mutex> lock(headMutex_);
            if (head_.get() == getTail())
                return std::unique_ptr<node>();
            x = std::move(*head_->val_);
            return popHead();
        }
    };
}

#endif