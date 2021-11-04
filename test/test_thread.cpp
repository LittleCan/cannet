#include <string>
#include <iostream>
#include "ThreadStd.h"
#include "CurrentThread.h"
#include <unistd.h>

using namespace std;

void threadFunc() {
    cout << cannet::tid() << endl;
//    cout << flush;
};

void threadFunc2(int n) {
    sleep(3);
    cout << cannet::tid() << " " << n << endl;
//    cout << flush;
}

int main() {
    cannet::ThreadStd t1(threadFunc);
    t1.start();
//    t1.join();
//    sleep(1);
    int n = 231;
    cannet::ThreadStd t2(std::bind(threadFunc2, n));
    t2.start();
    t2.join();
//    sleep(2);
    return 1;
}