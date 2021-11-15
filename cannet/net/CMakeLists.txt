project(cannet_net)
set(net_srcs
        Timer.cpp Timer.h HttpData.cpp HttpData.h Channel.cpp Channel.h EventLoop.cpp EventLoop.h Epoll.cpp Epoll.h Util.cpp Util.h Server.cpp Server.h EventLoopThreadPool.cpp EventLoopThreadPool.h EventLoopThread.cpp EventLoopThread.h)
add_library(${PROJECT_NAME} ${net_srcs})
target_link_libraries(
        cannet_net
        cannet_base
)
# add_library(${PROJECT_NAME} INTERfFACE)
target_include_directories(${PROJECT_NAME}
        PUBLIC ${PROJECT_SOURCE_DIR})


