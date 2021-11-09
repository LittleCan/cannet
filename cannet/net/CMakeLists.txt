project(cannet_net)
set(net_srcs
        File.cpp File.h Status.h Util.cpp Util.h ThreadPool.cpp ThreadPool.h Conf.cpp Conf.h PortPosix.cpp PortPosix.h Net.cpp Net.h Slice.h Poller.cpp Poller.h EventBase.cpp EventBase.h)
add_library(${PROJECT_NAME} ${net_srcs})
target_link_libraries(
        cannet_net
        cannet_base
)
# add_library(${PROJECT_NAME} INTERfFACE)
target_include_directories(${PROJECT_NAME}
        PUBLIC ${PROJECT_SOURCE_DIR})


