#ifndef CANNET_SERVER_H
#define CANNET_SERVER_H

#include <memory>
#include "Channel.h"
#include "EventLoop.h"
#include "EventLoopThreadPool.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <functional>
#include "Util.h"
#include "Logger.h"

namespace cannet {
    class Server {
    public:
        Server(EventLoop *loop, int threadNum, int port);

        ~Server() {}

        EventLoop *getLoop() const { return loop_; }

        void start();

        void handNewConn();

        void handThisConn() { loop_->updatePoller(acceptChannel_); }

    private:
        EventLoop *loop_;
        int threadNum_;
        std::unique_ptr<EventLoopThreadPool> eventLoopThreadPool_;
        bool started_;
        std::shared_ptr<Channel> acceptChannel_;
        int port_;
        int listenFd_;
        static const int MAXFDS = 100000;
    };

}


#endif //CANNET_SERVER_H
