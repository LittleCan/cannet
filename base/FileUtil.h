#ifndef CANNET_FILEUTIL_H
#define CANNET_FILEUTIL_H

#include <string>
#include <cstdio>

#include "Noncopyable.h"

namespace cannet {

    class AppendFile : private noncopyable{
    public:
        explicit AppendFile(const std::string &filename);

        ~AppendFile();

        void append(const char *logline, const size_t len);

        void flush();

    private:
        size_t write(const char *logline, const size_t len);

        FILE *fp_;

        char buffer_[64 * 1024];
    };
}


#endif //CANNET_FILEUTIL_H
