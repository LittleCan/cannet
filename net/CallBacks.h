#ifndef CANNET_NET_CALLBACKS_H
#define CANNET_NET_CALLBACKS_H

#include <functional>
#include <memory>

namespace cannet
{
    class Buffer;
    class TcpConnection;
    using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
    using ConnectionCallback = std::function<void(const TcpConnectionPtr &)>;
    using CloseCallback = std::function<void(const TcpConnectionPtr &)>;
    using WriteCompleteCallback = std::function<void(const TcpConnectionPtr &)>;
    using TimerCallback = std::function<void()>;
    using MessageCallback = std::function<void(const TcpConnectionPtr &,
                                               Buffer *)>;

    void defaultConnectionCallback(const TcpConnectionPtr &conn);
    void defaultMessageCallback(const TcpConnectionPtr &conn,
                                Buffer *buffer);
}

#endif