#ifndef CANNET_LOGFILE_H
#define CANNET_LOGFILE_H

#include <memory>
#include "Noncopyable.h"
#include <string>
#include <mutex>
#include "FileUtil.h"

namespace cannet {
    class LogFile : private noncopyable {
    public:
        LogFile(const std::string &basename, int flushEveryN = 1024);

        ~LogFile();

        void append(const char *logline, int len);

        void flush();

        bool rollFile();

    private:
        void append_unlocked(const char *logline, int len);

        const std::string basename_;
        const int flushEveryN_;
        int count_;
        std::unique_ptr<AppendFile> file_;
        std::unique_ptr<std::mutex> mutex_;
    };
}

#endif //CANNET_LOGFILE_H
