#ifndef CANNET_BASE_FILE
#define CANNET_BASE_FILE

#include <string>
#include <cassert>
#include "Noncopyable.h"

namespace cannet
{
    static const int kFileBufferSize = 64 * 1024;
    class WriteFile : noncopyable
    {
    private:
        size_t write(const char *log_, size_t len)
        {
            return ::fwrite_unlocked(log_, 1, len, fp_);
        }
        FILE *fp_;
        size_t writeLen_;
        char buffer_[kFileBufferSize];

    public:
        explicit WriteFile(std::string fileName);
        ~WriteFile();
        void append(const char *log_, size_t len);
        void flush() { ::fflush(fp_); };
        size_t writeLen() const { return writeLen_; }
    };

    class ReadSmallFile : noncopyable
    {
    public:
        explicit ReadSmallFile(std::string fileName);
        ~ReadSmallFile();
        int readToBuffer(ssize_t *size);

        const char *buffer() const { return buf_; }

    private:
        int fd_;
        int err_;
        char buf_[kFileBufferSize];
    };

    class SendFileUtil
    {
    public:
        explicit SendFileUtil(std::string fileName);
        explicit SendFileUtil();
        ~SendFileUtil();
        int getInId() const { return inFd_; }
        size_t &getSendLen() { return sendLen_; }
        off_t *getOffset() { return &offset_; }

    private:
        int inFd_;
        off_t offset_;
        size_t sendLen_;
        int err_;
    };
}

#endif