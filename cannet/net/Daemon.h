#ifndef CANNET_DAEMON_H
#define CANNET_DAEMON_H


#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <functional>
#include <map>
#include <string>
#include <utility>

namespace cannet {
    class Daemon {
    public:
        // exit in parent
        static int daemonStart(const char *pidfile);

        // exit in parent
        static int daemonRestart(const char *pidfile);

        static int daemonStop(const char *pidfile);

        static int getPidFromFile(const char *pidfile);

        // cmd: start stop restart
        // exit(1) when error
        // exit(0) when start or restart in parent
        // return when start or restart in child
        static void daemonProcess(const char *cmd, const char *pidfile);

        // fork; wait for parent to exit; exec argv
        // you may use it to implement restart in program
        static void changeTo(const char *argv[]);
    };

    class Signal {
    public:
        static void signal(int sig, const std::function<void()> &handler);
    };

}


#endif //CANNET_DAEMON_H
