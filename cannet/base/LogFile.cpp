#include "LogFile.h"

namespace cannet {
    LogFile::LogFile(const std::string &basename, int flushEveryN) :
            basename_(basename), flushEveryN_(flushEveryN), count_(0), mutex_(new std::mutex) {
        file_.reset(new AppendFile(basename));
    }

    LogFile::~LogFile() {}

    void LogFile::append(const char *logline, int len) {
        std::lock_guard<std::mutex> lock(*mutex_);
        append_unlocked(logline, len);
    }

    void LogFile::flush() {
        std::lock_guard<std::mutex> lock(*mutex_);
        file_->flush();
    }

    void LogFile::append_unlocked(const char *logline, int len) {
        file_->append(logline, len);
        ++count_;
        if (count_ >= flushEveryN_) {
            file_->flush();
            count_ = 0;
        }
    }
}