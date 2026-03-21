#pragma once

#include <iostream>
#include <source_location>

class Tracer {
public:
    explicit Tracer(
        const std::source_location &loc = std::source_location::current()) noexcept
        : loc_(loc) {
        std::cout << "[TRACE] START "
                << loc_.function_name()
                << "  ("
                << loc_.file_name()
                << ':'
                << loc_.line()
                << ':'
                << loc_.column()
                << ")\n";
    }

    ~Tracer() noexcept {
        std::cout << "[TRACE] FINISH "
                << loc_.function_name()
                << '\n';
    }

    Tracer(const Tracer &) = delete;

    Tracer &operator=(const Tracer &) = delete;

    Tracer(Tracer &&) = delete;

    Tracer &operator=(Tracer &&) = delete;

private:
    std::source_location loc_;
};

#ifdef NDEBUG
#   define trace()  ((void)0)
#else
#   define TRACER_CONCAT_IMPL(a, b)  a##b
#   define TRACER_CONCAT(a, b)       TRACER_CONCAT_IMPL(a, b)

#   define trace()  \
        ::Tracer TRACER_CONCAT(_tracer_, __LINE__)
#endif
