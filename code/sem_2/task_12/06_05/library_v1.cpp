#include <iostream>

extern "C" void test() {
    std::cout << "Library version 1: test()" << std::endl;
}