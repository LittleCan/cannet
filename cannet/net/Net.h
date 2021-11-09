#ifndef CANNET_NET_H
#define CANNET_NET_H

#include <netinet/in.h>
#include <string.h>
#include <algorithm>
#include <string>
#include "PortPosix.h"
#include "Slice.h"
#include "sys/socket.h"
#include <fcntl.h>
#include <netinet/tcp.h>
#include <cerrno>
#include "Util.h"
#include <iostream>

namespace cannet {
    class Net {
    public:
        template<class T>
        static T hton(T v) {
            return port::htobe(v);
        }

        template<class T>
        static T ntoh(T v) {
            return port::htobe(v);
        }

        static int setNonBlock(int fd, bool value = true);

        static int setReuseAddr(int fd, bool value = true);

        static int setReusePort(int fd, bool value = true);

        static int setNoDelay(int fd, bool value = true);
    };

    class Ip4Addr {
    public:
        Ip4Addr(const std::string &host, unsigned short port);

        Ip4Addr(unsigned short port = 0) : Ip4Addr("", port) {}

        Ip4Addr(const struct sockaddr_in &addr) : addr_(addr) {};

        std::string toString() const;

        std::string ip() const;

        unsigned short port() const;

        unsigned int ipInt() const;

        // if you pass a hostname to constructor, then use this to check error
        bool isIpValid() const;

        struct sockaddr_in &getAddr() {
            return addr_;
        }

        static std::string hostToIp(const std::string &host) {
            Ip4Addr addr(host, 0);
            return addr.ip();
        }

    private:
        struct sockaddr_in addr_;
    };

    class Buffer {
    public:
        Buffer() : buf_(nullptr), b_(0), e_(0), cap_(0), exp_(512) {}

        ~Buffer() { delete[] buf_; }

        void clear() {
            delete[] buf_;
            buf_ = nullptr;
            cap_ = 0;
            b_ = e_ = 0;
        }

        size_t size() const { return e_ - b_; }

        bool empty() const { return e_ == b_; }

        char *data() const { return buf_ + b_; }

        char *begin() const { return buf_ + b_; }

        char *end() const { return buf_ + e_; }

        char *makeRoom(size_t len);

        void makeRoom() {
            if (space() < exp_)
                expand(0);
        }

        size_t space() const { return cap_ - e_; }

        void addSize(size_t len) { e_ += len; }

        char *allocRoom(size_t len) {
            char *p = makeRoom(len);
            addSize(len);
            return p;
        }

        Buffer &append(const char *p, size_t len) {
            memcpy(allocRoom(len), p, len);
            return *this;
        }

        Buffer &append(Slice slice) { return append(slice.data(), slice.size()); }

        Buffer &append(const char *p) { return append(p, strlen(p)); }

        template<class T>
        Buffer &appendValue(const T &v) {
            append((const char *) &v, sizeof v);
            return *this;
        }

        Buffer &consume(size_t len) {
            b_ += len;
            if (size() == 0)
                clear();
            return *this;
        }

        Buffer &absorb(Buffer &buf);

        void setSuggestSize(size_t sz) { exp_ = sz; }

        Buffer(const Buffer &b) { copyFrom(b); }

        Buffer &operator=(const Buffer &b) {
            if (this == &b)
                return *this;
            delete[] buf_;
            buf_ = nullptr;
            copyFrom(b);
            return *this;
        }

        explicit operator Slice() { return {data(), size()}; }

    private:
        char *buf_;
        size_t b_, e_, cap_, exp_;

        void moveHead() {
            std::copy(begin(), end(), buf_);
            e_ -= b_;
            b_ = 0;
        }

        void expand(size_t len);

        void copyFrom(const Buffer &b);
    };
}


#endif //CANNET_NET_H
