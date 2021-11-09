#ifndef CANNET_EVENTBASE_H
#define CANNET_EVENTBASE_H

#include <memory>
#include <functional>
#include "Noncopyable.h"
#include "ThreadPool.h"
#include "Slice.h"

namespace cannet {
    struct Channel;
    struct TcpConn;
    struct TcpServer;
    struct IdleIdImp;
    struct EventsImp;
    struct EventBase;
    typedef std::unique_ptr<IdleIdImp> IdleId;
    typedef std::pair<int64_t, int64_t> TimerId;

    struct AutoContext : noncopyable {
        void *ctx;
        Task ctxDel;

        AutoContext() : ctx(nullptr) {}

        template<class T>
        T &context() {
            if (ctx == nullptr) {
                ctx = new T();
                ctxDel = [this] { delete (T *) ctx; };
            }
            return *(T *) ctx;
        }

        ~AutoContext() {
            if (ctx)
                ctxDel();
        }
    };

    typedef std::shared_ptr<TcpConn> TcpConnPtr;
    typedef std::shared_ptr<TcpServer> TcpServerPtr;
    typedef std::function<void(const TcpConnPtr &)> TcpCallBack;
    typedef std::function<void(const TcpConnPtr &, Slice msg)> MsgCallBack;
}


#endif //CANNET_EVENTBASE_H
