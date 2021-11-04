#include "Logger.h"
#include "ThreadStd.h"
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <unistd.h>
#include <chrono>


void threadFuncTest() {
    for (int i = 0; i < 5000; ++i) {
        LOG << i;
    }
}

void multiThreadFuncTest(int threadNum = 4) {
    std::vector<std::shared_ptr<cannet::ThreadStd>> v;
    for (int i = 0; i < threadNum; ++i) {
        auto temp = std::make_shared<cannet::ThreadStd>(threadFuncTest);
        v.push_back(temp);
    }
    for (const auto &i: v) {
        i->start();
    }
//    sleep(3);
}

void typetest() {
    LOG << 0;
    LOG << 0.1f;
    LOG << '0';
    LOG << "0123";
    LOG << (short) 0;
    LOG << (long) 0;
    LOG << (unsigned int) 0;
    LOG << (unsigned long) 0;
    LOG << std::string("string");
}

int main() {
    auto start = std::chrono::system_clock::now();
    multiThreadFuncTest(4);
    sleep(3);
    auto end = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << elapsed.count() << "ms\n";
//    threadFunc();
//
//    sleep(3);
//
//    multiThreadFuncTest(4);
//
//    sleep(3);
//
//    typetest();
//
//    sleep(3);
    return 0;
}