#include "Logger.h"
#include <iostream>

int main() {
    auto logger = new cannet::Logger(__FILE__, __LINE__);
    logger->m_time();
    logger->stream() << "abcd" << 123 << logger << '\n';
    delete logger;
    return 0;
}