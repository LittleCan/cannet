#include <sys/uio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <cstring>
#include <netinet/tcp.h>
#include <sys/sendfile.h>
#include "SocketOps.h"
#include "../base/Logger.h"

namespace cannet
{
    void socketops::bindOrDie(int sockfd, struct sockaddr_in *addr)
    {
        if (::bind(sockfd, reinterpret_cast<struct sockaddr *>(addr), static_cast<socklen_t>(sizeof(struct sockaddr_in))) < 0)
            LOG_SYSFATAL << "socketops error";
    }
    void socketops::listenOrDie(int sockfd)
    {
        if (::listen(sockfd, SOMAXCONN) < 0)
            LOG_SYSFATAL << "listen error";
    }

    int socketops::createStreamNonblockingOrDie() //创建非阻塞套接字，创建失败就终止
    {
        // linux 2.6.27以上的内核支持sock_nonblock和sock_cloexec的检测
        int sockfd = ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
        if (sockfd < 0)
        {
            LOG_SYSFATAL << "sockets::createStreamNonblockingOrDie";
        }
        return sockfd;
    }

    int socketops::createDgramNonblockingOrDie() //创建非阻塞套接字，创建失败就终止
    {
        // linux 2.6.27以上的内核支持sock_nonblock和sock_cloexec的检测
        int sockfd = ::socket(AF_INET, SOCK_DGRAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_UDP);
        if (sockfd < 0)
        {
            LOG_SYSFATAL << "sockets::createDgramNonblockingOrDie";
        }
        return sockfd;
    }

    int socketops::accept(int sockfd, struct sockaddr_in *addr)
    {
        auto addrlen = static_cast<socklen_t>(sizeof *addr);
        int connfd = ::accept4(sockfd, reinterpret_cast<struct sockaddr *>(addr),
                               &addrlen, SOCK_NONBLOCK | SOCK_CLOEXEC);
        if (connfd < 0 && errno != EAGAIN)
        {
            int savedErrno = errno; //先保存错误代码
            if (savedErrno == EAGAIN)
            {
                return connfd;
            }
            LOG_SYSERR << "Socket::accept"; //因为这里登记了一个错误，所以调用前先保存起来errno    }
            switch (savedErrno)
            {
                //        case EAGAIN:
            case ECONNABORTED:
            case EINTR:
            case EPERM:
            case EMFILE: // per-process lmit of open file desctiptor ???
                // expected errors
                errno = savedErrno; //将errno变回之前
                break;              //不是致命错误
            case EBADF:
            case EFAULT:
            case EINVAL:
            case ENFILE:
            case ENOBUFS:
            case ENOMEM:
            case ENOTSOCK:
            case EOPNOTSUPP:
                // 致命错误，登记日志，退出
                LOG_FATAL << "unexpected error of ::accept " << savedErrno;
                break;
            default: //未知错误，致命
                LOG_FATAL << "unknown error of ::accept " << savedErrno;
                break;
            }
        }
        return connfd;
    }

    int socketops::connect(int sockfd, struct sockaddr_in *addr)
    {
        return ::connect(sockfd, reinterpret_cast<struct sockaddr *>(addr), static_cast<socklen_t>(sizeof(struct sockaddr_in)));
    }

    ssize_t socketops::read(int sockfd, void *buf, size_t count)
    {
        return ::read(sockfd, buf, count);
    }

    ssize_t socketops::readv(int sockfd, const struct iovec *iov, int iovcnt)
    {
        return ::readv(sockfd, iov, iovcnt); //和read的区别是，接受的数据可以填充到多个缓冲区中
    }

    ssize_t socketops::write(int sockfd, const void *buf, size_t count)
    {
        return ::write(sockfd, buf, count); // writev没封装
    }

    ssize_t socketops::recvfrom(int sockfd, void *buf, size_t count, struct sockaddr_in *addr)
    {
        return ::recvfrom(sockfd, buf, count, 0, reinterpret_cast<sockaddr *>(addr),
                          reinterpret_cast<socklen_t *>(sizeof(struct sockaddr_in)));
    }
    ssize_t socketops::sendto(int sockfd, const void *buf, size_t count, struct sockaddr_in *addr)
    {
        return ::sendto(sockfd, buf, count, 0, reinterpret_cast<sockaddr *>(addr),
                        sizeof(struct sockaddr_in));
    }
    ssize_t socketops::sendfile(int outFd, int inFd, off_t *offset, size_t count)
    {
        return ::sendfile(outFd, inFd, offset, count); // sendfile没封装
    }

    void socketops::close(int sockfd)
    {
        if (::close(sockfd) < 0)
        {
            LOG_SYSERR << "sockets::close";
        }
    }
    //只关闭写的一段
    void socketops::shutdownWrite(int sockfd)
    {
        if (::shutdown(sockfd, SHUT_WR) < 0)
        {
            LOG_SYSERR << "sockets::shutdownWrite";
        }
    }

    int socketops::getSocketError(int sockfd)
    {
        int optval;
        auto optlen = static_cast<socklen_t>(sizeof optval);

        if (::getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &optval, &optlen) < 0)
        {
            return errno;
        }
        else
        {
            return optval;
        }
    }

    struct sockaddr_in socketops::getPeerAddr(int sockFd)
    {
        struct sockaddr_in peeraddr
        {
        };
        bzero(&peeraddr, sizeof peeraddr);
        auto addrLen = static_cast<socklen_t>(sizeof peeraddr);
        if (::getpeername(sockFd, reinterpret_cast<struct sockaddr *>(&peeraddr), &addrLen) < 0) //获取对方地址
        {
            LOG_SYSERR << "sockets::getPeerAddr";
        }
        return peeraddr;
    }

    struct sockaddr_in socketops::getLocalAddr(int sockFd)
    {
        struct sockaddr_in localaddr
        {
        };
        bzero(&localaddr, sizeof(localaddr));
        auto addrlen = static_cast<socklen_t>(sizeof localaddr);
        if (::getsockname(sockFd, reinterpret_cast<struct sockaddr *>(&localaddr), &addrlen) < 0)
        {
            LOG_SYSERR << "scokets::getLocalAddr";
        }
        return localaddr;
    }

    void socketops::setKeepAlive(int sockFd, bool on) //禁用或开启
    {
        int optval = on ? 1 : 0;
        ::setsockopt(sockFd, SOL_SOCKET, SO_KEEPALIVE,
                     &optval, static_cast<socklen_t>(sizeof optval));
    }

    void socketops::setTcpNoDelay(int sockFd, bool on) //禁用或开启
    {
        int optval = on ? 1 : 0;
        ::setsockopt(sockFd, IPPROTO_TCP, TCP_NODELAY,
                     &optval, static_cast<socklen_t>(sizeof optval));
    }

    void socketops::setReuseAddr(int sockFd, bool on) //禁用或开启
    {
        int optval = on ? 1 : 0;
        ::setsockopt(sockFd, SOL_SOCKET, SO_REUSEADDR,
                     &optval, static_cast<socklen_t>(sizeof optval));
    }

    void socketops::setReusePort(int sockFd, bool on) //禁用或开启
    {
        int optval = on ? 1 : 0;
        int ret = ::setsockopt(sockFd, SOL_SOCKET, SO_REUSEPORT,
                               &optval, static_cast<socklen_t>(sizeof optval));
        if (ret < 0 && on)
        {
            LOG_SYSERR << "SO_REUSEPORT failed.";
        }
    }
}