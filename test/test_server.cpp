#include <getopt.h>
#include <string>
#include "EventLoop.h"
#include "Server.h"
#include "Logger.h"

using namespace cannet;

int main() {
    int threadNum = 2;
    int port = 8000;
    std::string logPath = "./WebServer.log";

    Logger::setLogFileName(logPath);
// STL库在多线程上应用
#ifndef _PTHREADS
    LOG << "_PTHREADS is not defined !";
#endif
    EventLoop mainLoop;
    Server myHTTPServer(&mainLoop, threadNum, port);
    myHTTPServer.start();
    mainLoop.loop();
    return 0;
}