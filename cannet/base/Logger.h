#ifndef CANNET_BASE_LOGGER_H
#define CANNET_BASE_LOGGER_H

#include <string>
#include <iostream>

namespace cannet{
    class Logger
    {
    private:
        static std::string logFileName;
        
    public:
        Logger(const char *filename, int line);
        ~Logger();
        std::ostream &stream();
        static std::string getLogFileName(){return logFileName;};
    };

}

#endif