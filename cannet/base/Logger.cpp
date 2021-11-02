#include "Logger.h"

//todo add asynclogger

namespace cannet
{
    std::string Logger::logFileName_ = "./cannet.log";

    void Logger::Impl::formatTime() {
        struct timeval tv;
        time_t time;
        char str_t[26] = {0};
        gettimeofday (&tv, NULL);
        time = tv.tv_sec;
        struct tm* p_time = localtime(&time);
        strftime(str_t, 26, "%Y-%m-%d %H:%M:%S\n", p_time);
        stream_ << str_t;
    }
}