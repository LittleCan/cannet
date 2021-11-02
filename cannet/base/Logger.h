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

namespace cannet {
    class Logger {
    private:
        class Impl {
        public:
            Impl(const char *fileName, int line) : stream_(), line_(line), basename_(fileName) {};

            ~Impl() = default;

            void formatTime();

            LogStream stream_;
            int line_;
            std::string basename_;
        };

        Impl impl_;
        static std::string logFileName_;
    public:
        Logger(const char *filename, int line) : impl_(filename, line) {};

        ~Logger() {
            impl_.stream_ << " -- " << impl_.basename_ << ':' << impl_.line_ << '\n';
            std::cout << stream().buffer().data();
        };

        LogStream &stream() {
            return impl_.stream_;
        }

        void m_time() { impl_.formatTime(); }

        static void setLogFileName(std::string filename) {
            logFileName_ = std::move(filename);
        }

        static std::string getLogFileName() {
            return logFileName_;
        }
    };

}
#define LOG Logger(__FILE__,__LINE__).stream()
#endif