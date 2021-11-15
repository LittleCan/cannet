#ifndef CANNET_UTIL_H
#define CANNET_UTIL_H


#include <cstdlib>
#include <string>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <signal.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

namespace cannet {
    ssize_t readn(int fd, void *buff, size_t n);

    ssize_t readn(int fd, std::string &inBuffer, bool &zero);

    ssize_t readn(int fd, std::string &inBuffer);

    ssize_t writen(int fd, void *buff, size_t n);

    ssize_t writen(int fd, std::string &sbuff);

    void handle_for_sigpipe();

    int setSocketNonBlocking(int fd);

    void setSocketNodelay(int fd);

    void setSocketNoLinger(int fd);

    void shutDownWR(int fd);

    int socket_bind_listen(int port);

}
#endif //CANNET_UTIL_H
