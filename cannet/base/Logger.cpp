#include "Logger.h"

namespace cannet
{
    void Logger::log(LogLevel::Level level, LogEvent::ptr event)
    {
        if (level >= m_level)
        {
            for (auto &i : m_appenders)
            {
                i->log(level, event);
            }
        }
    };

    void Logger::debug(LogEvent::ptr event)
    {
        log(LogLevel::DEBUG, event);
    };
    void Logger::infor(LogEvent::ptr event)
    {
        log(LogLevel::INFO, event);
    };
    void Logger::warn(LogEvent::ptr event)
    {
        log(LogLevel::WARN, event);
    };
    void Logger::error(LogEvent::ptr event)
    {
        log(LogLevel::ERROR, event);
    };
    void Logger::fatal(LogEvent::ptr event)
    {
        log(LogLevel::FATAL, event);
    };
    void Logger::addAppender(LogAppender::ptr appender)
    {
        m_appenders.push_back(appender);
    };
    void Logger::delAppender(LogAppender::ptr appender)
    {
        for (auto it = m_appenders.begin(); it != m_appenders.end(); it++)
        {
            if (*it == appender)
            {
                m_appenders.erase(it);
                break;
            }
        }
    };

    void StdoutLogAppender::log(LogLevel::Level level, LogEvent::ptr event)
    {
        if (level >= m_level)
        {
            std::cout << m_formatter->format(event);
        }
    };

    void FileLogAppender::log(LogLevel::Level level, LogEvent::ptr event){
        if(level >= m_level){
            m_fileStream << m_formatter->format(event);
        }
    };

    bool FileLogAppender::reopen()
    {
        if (m_fileStream.is_open())
        {
            m_fileStream.close();
        }
        m_fileStream.open(m_name);
        return true;
    }
}