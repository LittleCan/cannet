#include "Logger.h"
#include "AsyncLogger.h"

namespace cannet {
    std::string Logger::logFileName_ = "./cannet.log";

    Logger::~Logger() {
        impl_.stream_ << " -- " << impl_.basename_ << ':' << impl_.line_ << '\n';
        const LogStream::Buffer &buf(stream().buffer());
        output(buf.data(), buf.length());
    }

    static pthread_once_t onceControl = PTHREAD_ONCE_INIT;
    static AsyncLogger *asyncLogger_;

    void Logger::Impl::formatTime() {
        struct timeval tv;
        time_t time;
        char str_t[26] = {0};
        gettimeofday(&tv, NULL);
        time = tv.tv_sec;
        struct tm *p_time = localtime(&time);
        strftime(str_t, 26, "%Y-%m-%d %H:%M:%S\n", p_time);
        stream_ << str_t;
    }

    void once_init() {
        asyncLogger_ = new AsyncLogger(Logger::getLogFileName());
        asyncLogger_->start();
    }

    void output(const char *msg, int len) {
        pthread_once(&onceControl, once_init);
        asyncLogger_->append(msg, len);
    }
}