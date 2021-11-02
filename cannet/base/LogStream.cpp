#include "LogStream.h"

namespace cannet {

    const char digits[] = "9876543210123456789";
    const char *zero = digits + 9;
    static_assert(sizeof(digits) == 20, "wrong number of digits");

    const char digitsHex[] = "0123456789ABCDEF";
    static_assert(sizeof digitsHex == 17, "wrong number of digitsHex");

// Efficient Integer to String Conversions, by Matthew Wilson.
    template<typename T>
    size_t convert(char buf[], T value) {
        T i = value;
        char *p = buf;

        do {
            int lsd = static_cast<int>(i % 10);
            i /= 10;
            *p++ = zero[lsd];
        } while (i != 0);

        if (value < 0) {
            *p++ = '-';
        }
        *p = '\0';
        std::reverse(buf, p);

        return p - buf;
    }

    size_t convertHex(char buf[], uintptr_t value) {
        uintptr_t i = value;
        char *p = buf;

        do {
            int lsd = static_cast<int>(i % 16);
            i /= 16;
            *p++ = digitsHex[lsd];
        } while (i != 0);

        *p = '\0';
        std::reverse(buf, p);

        return p - buf;
    }


    template
    class FixedBuffer<kSmallBuffer>;

    template
    class FixedBuffer<kLargeBuffer>;

    template<typename T>
    void LogStream::formatInteger(T val) {
        if (buffer_.avail() >= kMaxNumericSize) {
            size_t len = convert(buffer_.current(), val);
            buffer_.add(len);
        }
    }


    LogStream &LogStream::operator<<(bool val) {
        buffer_.append(val ? "1" : "0", 1);
        return *this;
    }

    LogStream &LogStream::operator<<(short val) {
        *this << static_cast<int>(val);
        return *this;
    }

    LogStream &LogStream::operator<<(unsigned short val) {
        *this << static_cast<unsigned int>(val);
        return *this;
    }

    LogStream &LogStream::operator<<(int val) {
        formatInteger(val);
        return *this;
    }

    LogStream &LogStream::operator<<(unsigned int val) {
        formatInteger(val);
        return *this;
    }

    LogStream &LogStream::operator<<(long val) {
        formatInteger(val);
        return *this;
    }

    LogStream &LogStream::operator<<(unsigned long val) {
        formatInteger(val);
        return *this;
    }

    LogStream &LogStream::operator<<(long long val) {
        formatInteger(val);
        return *this;
    }

    LogStream &LogStream::operator<<(unsigned long long val) {
        formatInteger(val);
        return *this;
    }

    LogStream &LogStream::operator<<(const void *val) {
        uintptr_t v = reinterpret_cast<uintptr_t >(val);
        if (buffer_.avail() >= kMaxNumericSize) {
            char *buf = buffer_.current();
            buf[0] = '0';
            buf[1] = 'x';
            size_t len = convertHex(buf + 2, v);
            buffer_.add(len + 2);
        }
        return *this;
    }

    LogStream &LogStream::operator<<(float val) {
        *this << static_cast<double >(val);
        return *this;
    }

    LogStream &LogStream::operator<<(double val) {
        if (buffer_.avail() >= kMaxNumericSize) {
            int len = snprintf(buffer_.current(), kMaxNumericSize, "%.12g", val);
            buffer_.add(len);
        }
        return *this;
    }


    LogStream &LogStream::operator<<(char val) {
        buffer_.append(&val, 1);
        return *this;
    }

    LogStream &LogStream::operator<<(const char *val) {
        if (val) {
            buffer_.append(val, strlen(val));
        } else {
            buffer_.append("(null)", 6);
        }
        return *this;
    }

    LogStream &LogStream::operator<<(const std::string &val) {
        buffer_.append(val.c_str(), val.size());
        return *this;
    }

    void LogStream::staticCheck() {
        static_assert(kMaxNumericSize - 10 > std::numeric_limits<double>::digits10,
                      "kMaxNumericSize is large enough");
        static_assert(kMaxNumericSize - 10 > std::numeric_limits<long double>::digits10,
                      "kMaxNumericSize is large enough");
        static_assert(kMaxNumericSize - 10 > std::numeric_limits<long>::digits10,
                      "kMaxNumericSize is large enough");
        static_assert(kMaxNumericSize - 10 > std::numeric_limits<long long>::digits10,
                      "kMaxNumericSize is large enough");
    }

}
