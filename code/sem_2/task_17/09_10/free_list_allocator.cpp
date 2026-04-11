#include "Allocator.hpp"

#include <cassert>
#include <memory>
#include <new>
#include <print>
#include <random>
#include <utility>
#include <vector>

#include <benchmark/benchmark.h>

class FreeListAllocator final : public Allocator {
public :
    enum class Policy { first, best };

    explicit FreeListAllocator(std::size_t size, Policy policy = Policy::first)
        : m_size(size), m_policy(policy) {
        assert(m_size >= sizeof(Node) + 1);

        m_begin = operator new(m_size, std::align_val_t(s_alignment));

        m_head = get<Node>(m_begin);
        m_head->size = m_size - sizeof(Header);
        m_head->next = nullptr;
    }

    ~FreeListAllocator() override {
        operator delete(m_begin, m_size, std::align_val_t(s_alignment));
    }

    auto allocate(std::size_t size, std::size_t = 0) -> void * override {
        void *end = get<std::byte>(m_begin) + sizeof(Header) + size, *next = end;

        auto free = 2 * alignof(Header);

        if (next = std::align(alignof(Header), sizeof(Header), next, free); next) {
            auto padding = get<std::byte>(next) - get<std::byte>(end);

            auto [current, previous] = (m_policy == Policy::first)
                                           ? find_first(size + padding)
                                           : find_best(size + padding);

            if (current) {
                if (current->size >= size + padding + sizeof(Node) + 1) {
                    auto step = sizeof(Header) + size + padding;
                    auto node = get<Node>(get<std::byte>(current) + step);
                    node->size = current->size - step;
                    node->next = current->next;
                    current->next = node;
                } else {
                    padding += current->size - size - padding;
                }

                if (!previous) m_head = current->next;
                else previous->next = current->next;

                get<Header>(current)->size = size + padding;

                return get<std::byte>(current) + sizeof(Header);
            }
        }

        return nullptr;
    }

    void deallocate(void *x) override {
        auto node = get<Node>(get<std::byte>(x) - sizeof(Header));

        Node *previous = nullptr, *current = m_head;

        while (current) {
            if (node < current) {
                node->next = current;

                if (!previous) m_head = node;
                else previous->next = node;

                break;
            }

            previous = current;
            current = current->next;
        }

        merge(previous, node);
    }

    void show() const override {
        std::print
        (
            "FreeListAllocator::show : m_size = {} m_begin = {:018} m_head = {:018}",
            m_size, m_begin, static_cast<void *>(m_head)
        );

        if (m_head->next)
            std::print(" m_head->next = {:018}\n", static_cast<void *>(m_head->next));
        else
            std::print("\n");
    }

private :
    struct Node {
        std::size_t size = 0;
        Node *next = nullptr;
    };

    struct alignas(std::max_align_t) Header {
        std::size_t size = 0;
    };

    auto find_first(std::size_t size) const -> std::pair<Node *, Node *> {
        Node *current = m_head, *previous = nullptr;

        while (current && size > current->size) {
            previous = current;
            current = current->next;
        }

        return std::make_pair(current, previous);
    }

    auto find_best(std::size_t size) const -> std::pair<Node *, Node *> {
        Node *best = nullptr, *best_previous = nullptr;
        Node *current = m_head, *previous = nullptr;

        while (current) {
            if (current->size >= size && (!best || current->size < best->size)) {
                best = current;
                best_previous = previous;
            }

            previous = current;
            current = current->next;
        }

        return std::make_pair(best, best_previous);
    }

    void merge(Node *previous, Node *node) const {
        if (node->next &&
            get<std::byte>(node) + sizeof(Header) + node->size ==
            get<std::byte>(node->next)) {
            node->size += sizeof(Header) + node->next->size;
            node->next = node->next->next;
        }

        if (previous &&
            get<std::byte>(previous) + sizeof(Header) + previous->size ==
            get<std::byte>(node)) {
            previous->size += sizeof(Header) + node->size;
            previous->next = node->next;
        }
    }

    std::size_t m_size = 0;
    FreeListAllocator::Policy m_policy;

    void *m_begin = nullptr;
    Node *m_head = nullptr;
};

static void bench_system(benchmark::State &state) {
    auto kb = 1uz << 10, mb = 1uz << 20;

    std::uniform_int_distribution distribution(1, 16);
    std::default_random_engine engine;

    std::vector<std::pair<void *, std::size_t> > vector(kb);

    for (auto _: state) {
        for (auto i = 0uz; i < kb; ++i) {
            vector[i].second = distribution(engine) * mb;
            vector[i].first = operator new(vector[i].second);
        }

        for (auto i = 0uz; i < kb; i += 32)
            operator delete(vector[i].first, vector[i].second);

        for (auto i = 0uz; i < kb; i += 32) {
            vector[i].second = distribution(engine) * mb;
            vector[i].first = operator new(vector[i].second);
        }

        for (auto i = 0uz; i < kb; ++i)
            operator delete(vector[i].first, vector[i].second);

        benchmark::DoNotOptimize(vector);
    }
}

static void run(benchmark::State &state, FreeListAllocator::Policy policy) {
    auto kb = 1uz << 10, mb = 1uz << 20, gb = 16uz << 30;

    std::uniform_int_distribution distribution(1, 16);
    std::default_random_engine engine;

    std::vector<void *> vector(kb, nullptr);

    for (auto _: state) {
        FreeListAllocator allocator(gb, policy);

        Allocator &a = allocator;

        for (auto i = 0uz; i < kb; ++i)
            vector[i] = a.allocate(distribution(engine) * mb);

        for (auto i = 0uz; i < kb; i += 32)
            a.deallocate(vector[i]);

        for (auto i = 0uz; i < kb; i += 32)
            vector[i] = a.allocate(distribution(engine) * mb);

        for (auto i = 0uz; i < kb; ++i)
            a.deallocate(vector[i]);

        benchmark::DoNotOptimize(vector);
    }
}

static void bench_first(benchmark::State &state) { run(state, FreeListAllocator::Policy::first); }
static void bench_best(benchmark::State &state) { run(state, FreeListAllocator::Policy::best); }

BENCHMARK(bench_system);
BENCHMARK(bench_first);
BENCHMARK(bench_best);

int main(int argc, char **argv) {
    for (auto policy: {FreeListAllocator::Policy::first, FreeListAllocator::Policy::best}) {
        FreeListAllocator allocator(1 << 10, policy);

        Allocator &a = allocator;

        a.show();
        a.allocate(16);
        a.show();
        auto x = a.allocate(16);
        a.show();
        auto y = a.allocate(16);
        a.show();
        a.allocate(16);

        a.show();
        a.deallocate(y);
        a.show();
        a.deallocate(x);

        a.show();
        auto z = a.allocate(32);
        a.show();

        assert(z == x);
    }

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    benchmark::Shutdown();
}
