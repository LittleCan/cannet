#ifndef CANNET_NONCOPYABLE_H
#define CANNET_NONCOPYABLE_H

namespace cannet{

    class noncopyable{
    public:
        noncopyable(const noncopyable&) = delete;
        void operator=(const noncopyable&) = delete;

    protected:
        noncopyable() = default;
        ~noncopyable() = default;
    };
}

#endif //CANNET_NONCOPYABLE_H
