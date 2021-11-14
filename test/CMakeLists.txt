project(cannet_test)

add_executable(test.out test_server.cpp)

target_link_libraries(test.out
    cannet_base
        cannet_net
)