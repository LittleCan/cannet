#include "Logger.h"

std::string cannet::Logger::logFileName = "ABC";

cannet::Logger::Logger(const char *filename, int line){

}

std::ostream & cannet::Logger::stream(){
    return std::cout;
}
