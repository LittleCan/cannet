project(cannet_test)

add_executable(test.out test_log.cpp)

target_link_libraries(test.out
    cannet_base
)