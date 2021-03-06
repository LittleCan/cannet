#ifndef CANNET_NET_SOCKETOPS_H
#define CANNET_NET_SOCKETOPS_H
#include <arpa/inet.h>
namespace cannet::socketops
{

    int createStreamNonblockingOrDie();                   //创建一个非阻塞的套接字
    int createDgramNonblockingOrDie();                    //创建非阻塞套接字，创建失败就终止
    int connect(int sockfd, struct sockaddr_in *addr);    //链接
    void bindOrDie(int sockfd, struct sockaddr_in *addr); //绑定监听
    void listenOrDie(int sockfd);
    int accept(int sockfd, struct sockaddr_in *addr);
    ssize_t read(int sockfd, void *buf, size_t count);
    ssize_t readv(int sockfd, const struct iovec *iov, int iovcnt);
    ssize_t write(int sockfd, const void *buf, size_t count);
    ssize_t recvfrom(int sockfd, void *buf, size_t count, struct sockaddr_in *addr);
    ssize_t sendto(int sockfd, const void *buf, size_t count, struct sockaddr_in *addr);
    ssize_t sendfile(int outFd, int inFd, off_t *offset, size_t count);
    void close(int sockfd);
    void shutdownWrite(int sockfd);
    void setKeepAlive(int sockFd, bool on);
    void setTcpNoDelay(int sockFd, bool on); //禁用或开启negle
    void setReuseAddr(int sockFd, bool on);  //地址复用,禁用或开启
    void setReusePort(int sockFd, bool on);
    int getSocketError(int sockfd);
    struct sockaddr_in getLocalAddr(int sockFd);
    struct sockaddr_in getPeerAddr(int sockFd);

}
#endif