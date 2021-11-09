#include "Util.h"

namespace cannet {

    std::string Util::format(const char *fmt, ...) {
        char buffer[500];
        std::unique_ptr<char[]> release1;
        char *base;
        for (int iter = 0; iter < 2; iter++) {
            int bufSize;
            if (iter == 0) {
                bufSize = sizeof(buffer);
                base = buffer;
            } else {
                bufSize = 30000;
                base = new char[bufSize];
                release1.reset(base);
            }

            char *p = base;
            char *limit = base + bufSize;
            if (p < limit) {
                va_list ap;
                va_start(ap, fmt);
                p += vsnprintf(p, limit - p, fmt, ap);
                va_end(ap);
            }
            if (p > limit) {
                if (iter == 0) continue;
                else {
                    p = limit - 1;
                    *p = '\0';
                }
            }
            break;
        }
        return base;
    }

    int64_t Util::timeMicro() {
        std::chrono::time_point<std::chrono::system_clock> p = std::chrono::system_clock::now();
        return std::chrono::duration_cast<std::chrono::microseconds>(p.time_since_epoch()).count();
    }

    int64_t Util::steadyMicro() {
        std::chrono::time_point<std::chrono::system_clock> p = std::chrono::system_clock::now();
        return std::chrono::duration_cast<std::chrono::microseconds>(p.time_since_epoch()).count();
    }

    std::string Util::readableTime(time_t t) {
        struct tm tm1{};
        localtime_r(&t, &tm1);
        return format("%04d-%02d-%02d %02d:%02d:%02d",
                      tm1.tm_year + 1900,
                      tm1.tm_mon + 1,
                      tm1.tm_mday,
                      tm1.tm_hour,
                      tm1.tm_min,
                      tm1.tm_sec
        );
    }

    int Util::addFdFlag(int fd, int flag) {
        int ret = fcntl(fd, F_GETFD);
        return fcntl(F_SETFD, ret | flag);
    }
}