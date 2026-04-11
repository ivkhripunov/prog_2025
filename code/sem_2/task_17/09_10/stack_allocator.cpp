#include "Allocator.hpp"

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <memory>
#include <new>
#include <print>
#include <vector>

#include <benchmark/benchmark.h>

class StackAllocator final : public Allocator {
public :
    explicit StackAllocator(std::size_t size) : m_size(size) {
        m_begin = operator new(m_size, std::align_val_t(s_alignment));
    }

    ~StackAllocator() override {
        operator delete(m_begin, m_size, std::align_val_t(s_alignment));
    }

    auto allocate(std::size_t size, std::size_t alignment = 0) -> void * override {
        if (!alignment) alignment = s_alignment;

        void *begin = get<std::byte>(m_begin) + m_offset + sizeof(header_t);

        auto free = m_size - m_offset - sizeof(header_t);

        if (begin = std::align(alignment, size, begin, free); begin) {
            auto header = get<header_t>(get<std::byte>(begin) - sizeof(header_t));

            *header = static_cast<header_t>(
                std::distance(get<std::byte>(m_begin) + m_offset,
                              get<std::byte>(begin))
            );

            m_offset = get<std::byte>(begin) - get<std::byte>(m_begin) + size;

            return begin;
        }

        return nullptr;
    }

    void deallocate(void *x) override {
        auto header = get<header_t>(get<std::byte>(x) - sizeof(header_t));

        m_offset = get<std::byte>(x) - get<std::byte>(m_begin) - *header;
    }

    void show() const override {
        std::print
        (
            "StackAllocator::show : m_size = {} m_begin = {:018} m_offset = {:0>4}\n",

            m_size, m_begin, m_offset
        );
    }

private :
    using header_t = std::uint8_t;

    std::size_t m_size = 0, m_offset = 0;

    void *m_begin = nullptr;
};

static void bench_system(benchmark::State &state) {
    auto kb = 1uz << 10, mb = 1uz << 20;

    std::vector<void *> vector(kb, nullptr);

    for (auto _: state) {
        for (auto i = 0uz; i < kb; ++i)
            vector[i] = operator new(mb);

        for (auto i = 0uz; i < kb; ++i)
            operator delete(vector[std::size(vector) - 1 - i], mb);

        benchmark::DoNotOptimize(vector);
    }
}

static void bench_custom(benchmark::State &state) {
    auto kb = 1uz << 10, mb = 1uz << 20, gb = 2uz << 30;

    std::vector<void *> vector(kb, nullptr);

    for (auto _: state) {
        StackAllocator allocator(gb);

        Allocator &a = allocator;

        for (auto i = 0uz; i < kb; ++i)
            vector[i] = a.allocate(mb);

        for (auto i = 0uz; i < kb; ++i)
            a.deallocate(vector[std::size(vector) - 1 - i]);

        benchmark::DoNotOptimize(vector);
    }
}

BENCHMARK(bench_system);
BENCHMARK(bench_custom);

int main(int argc, char **argv) {
    StackAllocator allocator(1 << 10);

    Allocator &a = allocator;

    a.show();
    a.allocate(1, 1);
    a.show();
    a.allocate(2, 2);
    a.show();
    auto x = a.allocate(4, 4);
    a.show();
    auto y = a.allocate(8, 8);

    a.show();
    a.deallocate(y);
    a.show();
    a.deallocate(x);

    a.show();
    auto z = a.allocate(8, 8);
    a.show();

    assert(z == x);

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    benchmark::Shutdown();
}
