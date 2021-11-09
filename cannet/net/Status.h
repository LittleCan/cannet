#ifndef CANNET_STATUS_H
#define CANNET_STATUS_H

#include <cerrno>
#include <cstdarg>
#include <cstring>
#include <string>
#include "Util.h"

namespace cannet {
    inline const char *errStr() {
        return strerror(errno);
    }

    class Status {
    private:
        const char *state_;

        static const char *copyState(const char *state);

    public:
        Status() : state_(nullptr) {};

        Status(int code, const char *msg);

        Status(int code, const std::string &msg) : Status(code, msg.c_str()) {};


        Status(const Status &s) : state_(copyState(s.state_)) {};

        Status(Status &&s) noexcept {
            state_ = s.state_;
            s.state_ = nullptr;
        }

        Status &operator=(const Status &s) {
            if (&s == this) return *this;
            delete[] state_;
            state_ = copyState(s.state_);
            return *this;
        }

        Status &operator=(Status &&s) noexcept {
            if (&s == this) return *this;
            delete[] state_;
            state_ = s.state_;
            s.state_ = nullptr;
            return *this;
        }

        ~Status() { delete[] state_; }

        static Status fromSystem() {
            return {errno, strerror(errno)};
        }

        static Status fromSystem(int err) {
            return {err, strerror(err)};
        }

        static Status fromFormat(int code, const char *fmt, ...);

        static Status ioError(const std::string &op, const std::string &name) {
            return Status::fromFormat(errno, "%s %s %s", op.c_str(), name.c_str(), errStr());
        }

        int code() {
            return state_ ? *(int32_t *) (state_ + 4) : 0;
        }

        const char *msg() {
            return state_ ? state_ + 8 : "";
        }

        bool ok() {
            return code() == 0;
        }

        std::string toString() {
            return Util::format("%d %s", code(), msg());
        }
    };

    inline const char *Status::copyState(const char *state) {
        if (state == nullptr) {
            return state;
        }
        uint32_t size = *(uint32_t *) state;
        char *res = new char[size];
        memcpy(res, state, size);
        return res;
    }

    inline Status::Status(int code, const char *msg) {
        uint32_t sz = strlen(msg) + 8;
        char *p = new char[sz];
        state_ = p;
        *(uint32_t *) p = sz;
        *(int32_t *) (p + 4) = code;
        memcpy(p + 8, msg, sz - 8);
    }

    inline Status Status::fromFormat(int code, const char *fmt, ...) {
        va_list ap;
        va_start(ap, fmt);
        uint32_t sz = 8 + vsnprintf(nullptr, 0, fmt, ap) + 1;
        va_end(ap);
        Status r;
        r.state_ = new char[sz];
        *(uint32_t *) r.state_ = sz;
        *(uint32_t *) (r.state_ + 4) = code;
        va_start(ap, fmt);
        vsnprintf((char *) (r.state_ + 8), sz - 8, fmt, ap);
        va_end(ap);
        return r;
    }
}

#endif //CANNET_STATUS_H
