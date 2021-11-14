#include "cannet.h"
#include <string>

int main() {
    cannet::EventBase base;
    cannet::Signal::signal(SIGINT, [&] { base.exit(); });
    cannet::TcpServerPtr server = cannet::TcpServer::startServer(&base, "", 2099);
    if (server == nullptr) std::cout << "error";
    server->onConnRead([](const cannet::TcpConnPtr &con) {
        auto s = con->getInput().data();
        std::cout << s << std::flush;
        con->send(s);
    });
    base.loop();
    return 0;
}