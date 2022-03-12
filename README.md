# 自己编写的网络库
一个参考muduo编写的linux网络库，基于Reactor模式、one loop per thread。
## 目前实现的情况
1. 仅使用C++ 11 std，没有其他依赖。
2. 使用epoll构建的tcpserver。
3. 使用std:thread、std:bind和std:function构建线程池。
4. 使用std:chrono和timerfd完成定时器。
5. 线程安全的日志。

## 下一步要实现的目标
1. 跨平台
2. httpserver
3. udpserver
4. 动态加载配置
5. 数据库的接口
