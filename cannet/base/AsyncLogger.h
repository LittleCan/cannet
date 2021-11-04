#ifndef CANNET_ASYNCLOGGER_H
#define CANNET_ASYNCLOGGER_H

#include "Noncopyable.h"
#include <functional>
#include <string>
#include <vector>
#include "LogStream.h"
#include "ThreadStd.h"
#include "CountDownLatch.h"
#include "LogFile.h"

namespace cannet {
    class AsyncLogger : private noncopyable {
    public:
        AsyncLogger(const std::string basename, int flushInterval = 2);

        ~AsyncLogger() {
            if (running_)
                stop();
        }

        void append(const char *logline, int len);
        void start(){
            running_ = true;
            thread_.start();
            latch_.wait();
        }
        void stop(){
            running_ = false;
            cond_.notify_one();
            thread_.join();
        }

    private:
        void threadFun();

        typedef FixedBuffer<kLargeBuffer> Buffer;
        typedef std::vector<std::shared_ptr<Buffer>> BufferVector;
        typedef std::shared_ptr<Buffer> BufferPtr;
        const int flushInterval_;
        bool running_;
        std::string basename_;
        ThreadStd thread_;
        std::mutex mutex_;
        std::condition_variable cond_;
        BufferPtr currentBuffer_;
        BufferPtr nextBuffer_;
        BufferVector buffers_;
        CoutDownLatch latch_;
    };
}

#endif //CANNET_ASYNCLOGGER_H
