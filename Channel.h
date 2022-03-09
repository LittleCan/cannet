#ifndef CANNET_NET_CHANNEL_H
#define CANNET_NET_CHANNEL_H

#include <functional>
#include <memory>
#include <string>
#include "base/Noncopyable.h"

namespace cannet
{

    class EventLoop;
    class TcpConnection;

    extern const unsigned kNoneEvent;
    extern const unsigned kReadEventLT;
    extern const unsigned kReadEventET;
    extern const unsigned kWriteEvent;

    class Channel : noncopyable
    {
    public:
        using EventCallback = std::function<void()>;

        explicit Channel(EventLoop *loop, int fd);

        ~Channel();

        void handleEvent();

        void setReadCallback(EventCallback cb)
        {
            readCallback_ = std::move(cb);
        }

        void setWriteCallback(EventCallback cb)
        {
            writeCallback_ = std::move(cb);
        }

        void setCloseCallback(EventCallback cb)
        {
            closeCallback_ = std::move(cb);
        }

        void setErrorCallback(EventCallback cb)
        {
            errorCallback_ = std::move(cb);
        }

        int fd() const { return fd_; }

        unsigned events() const { return events_; }

        void setRevents(unsigned revents) { revents_ = revents; }

        void enableEvents(unsigned events)
        {
            events_ |= events;
            update();
        }

        void disableEvents(unsigned events)
        {
            events_ &= ~events;
            update();
        }

        void disableAll()
        {
            events_ = kNoneEvent;
            update();
        }

        bool isNoneEvent() const
        {
            return events_ == kNoneEvent;
        }
        bool isWriting() const
        {
            return events_ & kWriteEvent;
        }
        bool isReading() const
        {
            return events_ & kReadEventLT;
        }

        int status() const
        {
            return status_;
        }
        void setStatus(int status) { status_ = status; }

        EventLoop *ownerLoop()
        {
            return loop_;
        }
        void remove();
        void channelReset(int socketId);

    private:
        void update();

        EventLoop *loop_;
        int fd_;
        unsigned events_;
        unsigned revents_;
        int status_;
        bool eventHandling_;
        bool addedToLoop_;
        EventCallback readCallback_;
        EventCallback writeCallback_;
        EventCallback closeCallback_;
        EventCallback errorCallback_;
    };
}

#endif