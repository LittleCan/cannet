project(cannet_base)
set(base_srcs
        Logger.cpp
        Logger.h
        Noncopyable.h
        Copyable.h
        LogStream.h
        LogStream.cpp)
add_library(${PROJECT_NAME} ${base_srcs})
# add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME}
        PUBLIC ${PROJECT_SOURCE_DIR}
        )

