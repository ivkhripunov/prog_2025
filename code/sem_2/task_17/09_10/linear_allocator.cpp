#include "Allocator.hpp"

#include <cstddef>
#include <memory>
#include <new>
#include <print>
#include <vector>

#include <benchmark/benchmark.h>

class linear_allocator final : public Allocator {
public :
    explicit linear_allocator(std::size_t size) : m_size(size) {
        m_begin = operator new(m_size, std::align_val_t(s_alignment));
    }

    ~linear_allocator() override {
        operator delete(m_begin, m_size, std::align_val_t(s_alignment));
    }

    auto allocate(std::size_t size, std::size_t alignment = 0) -> void * override {
        if (!alignment) alignment = s_alignment;

        void *begin = get<std::byte>(m_begin) + m_offset;

        auto free = m_size - m_offset;

        if (begin = std::align(alignment, size, begin, free); begin) {
            m_offset = m_size - free + size;

            return begin;
        }

        return nullptr;
    }

    void deallocate(void *) override {
    }

    void show() const override {
        std::print
        (
            "LinearAllocator::show : m_size = {} m_begin = {:018} m_offset = {:0>4}\n",

            m_size, m_begin, m_offset
        );
    }

private :
    std::size_t m_size = 0, m_offset = 0;

    void *m_begin = nullptr;
};

static void bench_system(benchmark::State &state) {
    auto kb = 1uz << 10, mb = 1uz << 20;

    std::vector<void *> vector(kb, nullptr);

    for (auto _: state) {
        for (auto i = 0uz; i < kb; ++i) vector[i] = operator new(mb);
        for (auto i = 0uz; i < kb; ++i) operator delete(vector[i], mb);

        benchmark::DoNotOptimize(vector);
    }
}

static void bench_custom(benchmark::State &state) {
    auto kb = 1uz << 10, mb = 1uz << 20, gb = 1uz << 30;

    std::vector<void *> vector(kb, nullptr);

    for (auto _: state) {
        linear_allocator allocator(gb);

        Allocator &a = allocator;

        for (auto i = 0uz; i < kb; ++i) vector[i] = a.allocate(mb);

        benchmark::DoNotOptimize(vector);
    }
}

BENCHMARK(bench_system);
BENCHMARK(bench_custom);

int main(int argc, char **argv) {
    linear_allocator allocator(1 << 10);

    allocator.show();
    allocator.allocate(1, 1);
    allocator.show();
    allocator.allocate(2, 2);
    allocator.show();
    allocator.allocate(4, 4);
    allocator.show();
    allocator.allocate(8, 8);
    allocator.show();

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    benchmark::Shutdown();
}
