#include "tracer.hpp"
#include <stdexcept>

void direct_usage() {
    Tracer t;
    std::cout << "тело функции\n";
}

void macro_usage() {
    trace();
    std::cout << "тело функции\n";
}

void early_return(const bool flag) {
    trace();
    if (flag) {
        std::cout << "ранний выход\n";
        return;
    }
    std::cout << "нормальный выход\n";
}

void throwing_func() {
    trace();
    throw std::runtime_error("тестовое исключение");
}

void inner() {
    trace();
    std::cout << "тело функции\n";
}

void outer() {
    trace();
    std::cout << "вызывается inner()\n";
    inner();
    std::cout << "inner() завершилась\n";
}

int main() {
    direct_usage();

    macro_usage();

    early_return(true);

    early_return(false);

    try {
        throwing_func();
    } catch (const std::exception &e) {
        std::cout << "поймано: " << e.what() << '\n';
    }

    outer();

    return 0;
}
