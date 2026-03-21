#include <stdexcept>

#include "tracer.hpp"

void direct_usage() {
    Tracer t;
    std::cout << "function internals\n";
}

void macro_usage() {
    trace();
    std::cout << "function internals\n";
}

void early_return(const bool flag) {
    trace();
    if (flag) {
        std::cout << "early exit\n";
        return;
    }
    std::cout << "normal exit\n";
}

void throwing_func() {
    trace();
    throw std::runtime_error("test exception");
}

void inner() {
    trace();
    std::cout << "function internals\n";
}

void outer() {
    trace();
    std::cout << "call inner()\n";
    inner();
    std::cout << "inner() finished\n";
}

int main() {
    direct_usage();

    macro_usage();

    early_return(true);

    early_return(false);

    try {
        throwing_func();
    } catch (const std::exception &e) {
        std::cout << "catched: " << e.what() << '\n';
    }

    outer();

    return 0;
}
