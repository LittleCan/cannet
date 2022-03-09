#ifndef CANNET_BASE_LOGGER_H
#define CANNET_BASE_LOGGER_H

#include <string>
#include <iostream>
#include <string>
#include <memory>
#include <utility>
#include "LogStream.h"
#include <ctime>
#include "sys/time.h"
#include <pthread.h>

namespace cannet
{
    class Logger
    {
    private:
        class Impl
        {
        public:
            Impl(const char *fileName, int line) : stream_(), line_(line), basename_(fileName)
            {
                formatTime();
            };

            ~Impl() = default;

            void formatTime();

            LogStream stream_;
            int line_;
            std::string basename_;
        };

        Impl impl_;
        static std::string logFileName_;

    public:
        Logger(const char *filename, int line) : impl_(filename, line){};

        ~Logger();

        LogStream &stream()
        {
            return impl_.stream_;
        }

        static void setLogFileName(std::string filename)
        {
            logFileName_ = std::move(filename);
        }

        static std::string getLogFileName()
        {
            return logFileName_;
        }
    };

    void log_exit();

    void once_init();

    void output(const char *msg, int len);

}
#define LOG cannet::Logger(__FILE__, __LINE__).stream()
#define LOG_WARN cannet::Logger(__FILE__, __LINE__).stream()
#define LOG_ERROR cannet::Logger(__FILE__, __LINE__).stream()
#define LOG_FATAL cannet::Logger(__FILE__, __LINE__).stream()
#define LOG_SYSERR cannet::Logger(__FILE__, __LINE__).stream()
#define LOG_SYSFATAL cannet::Logger(__FILE__, __LINE__).stream()
#endif