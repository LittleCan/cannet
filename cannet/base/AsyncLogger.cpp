#include "AsyncLogger.h"

namespace cannet {
    AsyncLogger::AsyncLogger(const std::string basename, int flushInterval) :
            flushInterval_(flushInterval), running_(false), basename_(basename),
            thread_(std::bind(&AsyncLogger::threadFun, this), "Logging"),
            currentBuffer_(new Buffer), nextBuffer_(new Buffer), buffers_(), latch_(1) {
        assert(basename_.size() > 1);
        currentBuffer_->resetData();
        nextBuffer_->resetData();
        buffers_.reserve(16);
    }

    void AsyncLogger::append(const char *logline, int len) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (currentBuffer_->avail() > len) {
            currentBuffer_->append(logline, len);
        } else {
            buffers_.push_back(currentBuffer_);
            currentBuffer_.reset();
            if (nextBuffer_)
                currentBuffer_ = std::move(nextBuffer_);
            else
                currentBuffer_.reset(new Buffer);
            currentBuffer_->append(logline, len);
            cond_.notify_all();
        }
    }

    void AsyncLogger::threadFun() {
        assert(running_ == true);
        latch_.countDown();
        LogFile output(basename_);
        BufferPtr newBuffer1(new Buffer);
        BufferPtr newBuffer2(new Buffer);
        newBuffer1->resetData();
        newBuffer2->resetData();
        BufferVector bufferToWrite;
        bufferToWrite.reserve(16);
        while (running_) {
            assert(newBuffer1 && newBuffer1->length() == 0);
            assert(newBuffer2 && newBuffer2->length() == 0);
            assert(bufferToWrite.empty());
            {
                std::unique_lock<std::mutex> lock(mutex_);
                if (buffers_.empty()) {
                    cond_.wait_for(lock, std::chrono::seconds(flushInterval_));
                }
                buffers_.push_back(currentBuffer_);
                currentBuffer_.reset();
                currentBuffer_ = std::move(newBuffer1);
                bufferToWrite.swap(buffers_);
                if (!nextBuffer_) {
                    nextBuffer_ = std::move(newBuffer2);
                }
            }
            assert(!bufferToWrite.empty());
            if (bufferToWrite.size() > 25) {
                bufferToWrite.erase(bufferToWrite.begin() + 2, bufferToWrite.end());
            }
            for (auto &i: bufferToWrite) {
                output.append(i->data(), i->length());
            }
            if (bufferToWrite.size() > 2) {
                bufferToWrite.resize(2);
            }
            if (!newBuffer1) {
                assert(!bufferToWrite.empty());
                newBuffer1 = bufferToWrite.back();
                bufferToWrite.pop_back();
                newBuffer1->reset();
            }

            if (!newBuffer2) {
                assert(!bufferToWrite.empty());
                newBuffer2 = bufferToWrite.back();
                bufferToWrite.pop_back();
                newBuffer2->reset();
            }
            bufferToWrite.clear();
            output.flush();
        }
        output.flush();
    }
}