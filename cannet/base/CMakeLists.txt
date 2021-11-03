project(cannet_base)
set(base_srcs
        Logger.cpp
        Logger.h
        Noncopyable.h
        Copyable.h
        LogStream.h
        LogStream.cpp AsyncLogger.h Thread.h CountDownLatch.h CountDownLatch.cpp Thread.cpp CurrentThread.h AsyncLogger.cpp LogFile.cpp LogFile.h FileUtil.cpp FileUtil.h)
add_library(${PROJECT_NAME} ${base_srcs})
# add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME}
        PUBLIC ${PROJECT_SOURCE_DIR}
        )

