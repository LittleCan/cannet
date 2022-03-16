# 自己编写的网络库
一个参考muduo编写的c++ 11 linux网络库，基于Reactor模式、one loop per thread。

详细的功能注释可以看[模块功能注释](https://github.com/LittleCan/cannet/blob/main/md/%E6%A8%A1%E5%9D%97%E5%8A%9F%E8%83%BD%E6%B3%A8%E9%87%8A.md)
## 特点
1. 使用了one-loop-per-thread模型
2. 主线程只进行 accept 操作通过 eventfd 进行事件分发，避免锁的竞争，IO 线程进行数据读写.
3. 所有事件循环都使用了epoll多路复用,提高性能.
4. 使用 RAII 的机制进行对象生命周期控制,所有内存分配操作使用智能指针,避免了内存泄露.
5. 使用 Linux 内核提供的 timerfd 将定时事件和 IO 事件统一处理, 通过 C++11 标准库 std::chrono 和 std::priority_queue 实现定时器管理,实现纳秒级别定时任务
6. 封装高性能 buffer 类进行数据发送和接收.
7. 文件发送使用 sendfile 零拷贝技术，提高文件发送性能.
8. 实现了多缓冲区异步日志库
9. 多处设计进行对象复用,减少内存频繁申请释放.
10. 仅使用C++ 11 std与linux 库函数，没有其他依赖。

## 下一步要实现的目标
1. 跨平台
2. httpserver
3. udpserver
4. 动态加载配置
5. 数据库的接口
6. 
