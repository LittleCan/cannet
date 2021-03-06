#ifndef CANNET_NET_CONNECTION_H
#define CANNET_NET_CONNECTION_H
#include <memory>
#include <netinet/in.h>
#include <functional>
#include <string_view>
#include <atomic>
#include "../base/Noncopyable.h"
#include "Buffer.h"
#include "CallBacks.h"
#include "../base/File.h"
namespace cannet
{
    class EventLoop;
    class Channel;

    class TcpConnection : private noncopyable,
                          public std::enable_shared_from_this<TcpConnection>
    {
    public:
        explicit TcpConnection(EventLoop *loop,
                               int sockFd);
        ~TcpConnection();

        EventLoop *getLoop() const { return loop_; }

        bool connected() const { return state_ == kConnected; }
        void send(std::string message);
        void send(Buffer *buf);
        void shutdown();
        void forceClose();
        void setTcpNoDelay(bool on) const;
        void startRead();
        void stopRead();
        bool isReading() const { return reading_; }
        bool isSendFile() const { return sendFile_ != nullptr; }
        void sendFileReset()
        {
            if (sendFile_ != nullptr)
                sendFile_.reset();
        };

        void setMessageCallback(const MessageCallback &cb) { messageCallback_ = cb; }
        void setWriteCompleteCallback(const WriteCompleteCallback &cb) { writeCompleteCallback_ = cb; }
        void setConnectionCallback(const ConnectionCallback &cb) { connectionCallback_ = cb; }
        std::unique_ptr<Channel> &getChannel() { return channel_; }
        void setSendFile(std::unique_ptr<SendFileUtil> file) { sendFile_ = std::move(file); }
        Buffer *inputBuffer() { return &inputBuffer_; }
        Buffer *outputBuffer() { return &outputBuffer_; }
        void setCloseCallback(const CloseCallback &cb) { closeCallback_ = cb; }

        void connectEstablished();
        void connectDestroyed();
        void connectReset(int sockFd);

    private:
        enum StateE //连接的状态
        {
            kDisconnected,
            kConnecting,
            kConnected,
            kDisconnecting
        };
        void handleRead();
        void handleWrite();
        void handleClose();
        void handleError();

        void sendInLoop(const std::string &message, size_t len);
        void shutdownInLoop();
        void forceCloseInLoop();

        void setState(StateE s) { state_ = s; }
        void startReadInLoop();
        void stopReadInLoop();

        EventLoop *loop_; // 所属eventLoop
        std::atomic_uint8_t state_;
        int sockFd_;
        std::unique_ptr<Channel> channel_;
        ConnectionCallback connectionCallback_;
        MessageCallback messageCallback_;
        WriteCompleteCallback writeCompleteCallback_;
        CloseCallback closeCallback_;

        Buffer inputBuffer_;  //应用层的接收缓冲区
        Buffer outputBuffer_; //应用层的发送缓冲区，当outputbuffer高到一定程度
        std::unique_ptr<SendFileUtil> sendFile_;
        bool reading_;
    };

    typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
} // namespace ssxrver

#endif