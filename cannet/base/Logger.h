#ifndef CANNET_BASE_LOGGER_H
#define CANNET_BASE_LOGGER_H

#include <string>
#include <iostream>
#include <stdint.h>
#include <string>
#include <memory>

namespace cannet
{
    class LogLevel
    {
    public:
        enum Level
        {
            DEBUG = 1,
            INFO = 2,
            WARN = 3,
            ERROR = 4,
            FATAL = 5
        };
    };
    class LogEvent
    {
    public:
        LogEvent() = default;
        typedef std::shared_ptr<LogEvent> ptr;

    private:
        const char *m_file = nullptr;
        int32_t m_line = 0;
        uint32_t m_threadId = 0;
        uint32_t m_time;
        std::string m_content;
    };

    class LogFormatter
    {
    public:
        typedef std::shared_ptr<LogFormatter> ptr;
        std::string format(LogEvent::ptr event);
    };
    class LogAppender
    {
    public:
        virtual ~LogAppender(){};
        typedef std::shared_ptr<LogAppender> ptr;
        void log(LogLevel::Level level, LogEvent::ptr event);

    private:
        LogLevel::Level m_level;
    };

    class Logger
    {
    public:
        Logger(const std::string &name = "root") : m_name(name){};
        ~Logger() = default;
        typedef std::shared_ptr<Logger> ptr;

        void log(LogLevel::Level level, const LogEvent &event);

    private:
        std::string m_name;
        LogAppender::ptr m_appender;
        LogLevel::Level m_level;
    };

    class StdoutLogAppender: public LogAppender{

    };

    class FileLogAppender: public LogAppender{

    };

}

#endif