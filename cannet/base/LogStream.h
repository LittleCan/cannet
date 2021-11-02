#ifndef CANNET_LOGSTREAM_H
#define CANNET_LOGSTREAM_H

#include "Noncopyable.h"
#include <string>
#include <cstring>
#include <cassert>
#include <algorithm>
#include <cstdint>
#include <limits>


const int kSmallBuffer = 4000;
const int kLargeBuffer = 4000 * 1000;

namespace cannet {
    template<int SIZE>
    class FixedBuffer : private noncopyable {
    private:
        char data_[SIZE];
        char *cur_;

        const char *end() const { return data_ + sizeof(data_); };
    public:
        FixedBuffer() : cur_(data_) {}

        ~FixedBuffer() = default;

        int avail() const {
            return static_cast<int>(end() - cur_);
        }


        void append(const char *buf, size_t len) {
            if (avail() > static_cast<int>(len)) {
                memcpy(cur_, buf, len);
                cur_ += len;
            }
        }

        const char *data() const {
            return data_;
        }

        int length() const {
            return static_cast<int> (cur_ - data_);
        }

        char *current() {
            return cur_;
        }

        void add(size_t len) {
            cur_ += len;
        }

        void resetData() {
            memset(data_, 0, sizeof(data_));
        }

    };

    class LogStream : private noncopyable {

    public:
        typedef FixedBuffer<kSmallBuffer> Buffer;

        LogStream &operator<<(bool);

        LogStream &operator<<(short);

        LogStream &operator<<(unsigned short);

        LogStream &operator<<(int);

        LogStream &operator<<(unsigned int);

        LogStream &operator<<(long);

        LogStream &operator<<(unsigned long);

        LogStream &operator<<(long long);

        LogStream &operator<<(unsigned long long);

        LogStream &operator<<(const void *);

        LogStream &operator<<(float);

        LogStream &operator<<(double);

        LogStream &operator<<(char);

        LogStream &operator<<(const char *);

        LogStream &operator<<(const std::string &);

        void append(const char *data, int len) {
            buffer_.append(data, len);
        }

        const Buffer &buffer() {
            return buffer_;
        }

        void resetBuffer() {
            buffer_.resetData();
        }


    private:
        void staticCheck();

        template<typename T>
        void formatInteger(T);

        Buffer buffer_;

        static const int kMaxNumericSize = 48;
    };

}

#endif //CANNET_LOGSTREAM_H
