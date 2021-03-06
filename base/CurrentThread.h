#ifndef CANNET_BASE_CURRENTTHREAD_H
#define CANNET_BASE_CURRENTTHREAD_H

#include <sys/syscall.h>
#include <sys/types.h>
#include <string>
#include <unistd.h>

namespace cannet::CurrentThread
{
    extern thread_local int t_cachedTid;
    extern thread_local std::string t_tidString;

    inline int tid(){
        if(t_cachedTid == 0){
            t_cachedTid = static_cast<pid_t>(syscall(SYS_gettid));
            t_tidString = std::to_string(t_cachedTid) + " ";
        }
        return t_cachedTid;
    }

    inline const char *tidString(){
        return t_tidString.c_str();
    }

    bool isMainThread();
}

#endif