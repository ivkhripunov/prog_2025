#include <iostream>

extern "C" void test() {
    std::cout << "Library version 2: test()" << std::endl;
}