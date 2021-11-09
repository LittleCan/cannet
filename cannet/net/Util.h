#ifndef CANNET_UTIL_H
#define CANNET_UTIL_H

#include <cstdlib>
#include <cstring>
#include <functional>
#include <string>
#include <utility>
#include <fcntl.h>
#include <chrono>
#include <memory>
#include <algorithm>
#include <cstdarg>
#include "Noncopyable.h"

namespace cannet {
    class Util {
    public:
        static std::string format(const char *fmt, ...);

        static int64_t timeMicro();

        static int64_t timeMilli() {
            return timeMicro() / 1000;
        }

        static int64_t steadyMicro();

        static int64_t steadyMilli() {
            return steadyMicro() / 1000;
        }

        static std::string readableTime(time_t t);

        static int64_t atoi(const char *b, const char *e) {
            return strtol(b, (char **) &e, 10);
        }

        static int64_t atoi2(const char *b, const char *e) {
            char *ne = nullptr;
            int64_t v = strtol(b, &ne, 10);
            return ne == e ? v : -1;
        }

        static int64_t atoi(const char *b) {
            return atoi(b, b + strlen(b));
        }

        static int addFdFlag(int fd, int flag);
    };

    class ExitCaller : private noncopyable {
    public:
        ExitCaller(std::function<void()> &&functor) : functor_(std::move(functor)) {};

        ~ExitCaller() { functor_(); }

    private:
        std::function<void()> functor_;
    };
}


#endif //CANNET_UTIL_H
