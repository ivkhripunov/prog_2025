#include "Allocator.hpp"

#include <cassert>
#include <new>
#include <print>
#include <vector>

#include <benchmark/benchmark.h>

class ListAllocator final : public Allocator {
public :
    explicit ListAllocator(std::size_t size, std::size_t step)
        : m_size(size), m_step(step) {
        assert(m_size % m_step == 0 && m_step >= sizeof(Node));

        make_list();

        m_begin = m_head;
    }

    ~ListAllocator() override {
        for (auto list: m_lists)
            operator delete(list, m_size, std::align_val_t(s_alignment));
    }

    auto allocate(std::size_t = 0, std::size_t = 0) -> void * override {
        if (!m_head) {
            if (m_offset == std::size(m_lists))
                make_list();
            else
                m_head = get<Node>(m_lists[++m_offset - 1]);
        }

        auto node = m_head;

        if (!node->next) {
            auto next = get<std::byte>(node) + m_step;

            if (next != get<std::byte>(m_lists[m_offset - 1]) + m_size) {
                m_head = get<Node>(next);
                m_head->next = nullptr;
            } else {
                m_head = m_head->next;
            }
        } else {
            m_head = m_head->next;
        }

        return node;
    }

    void deallocate(void *x) override {
        auto node = get<Node>(x);

        node->next = m_head;

        m_head = node;
    }

    void show() const override {
        std::print
        (
            "ListAllocator::show : m_size = {} m_step = {} m_begin = {:018} "
            "m_head = {:018} m_offset = {}\n",
            m_size, m_step, m_begin, static_cast<void *>(m_head), m_offset
        );
    }

private :
    struct Node {
        Node *next = nullptr;
    };

    void make_list() {
        m_head = get<Node>(operator new(m_size, std::align_val_t(s_alignment)));
        m_head->next = nullptr;
        ++m_offset;
        m_lists.push_back(m_head);
    }

    std::size_t m_size = 0, m_step = 0, m_offset = 0;

    void *m_begin = nullptr;

    Node *m_head = nullptr;

    std::vector<void *> m_lists;
};

static void bench_system(benchmark::State &state) {
    auto kb = 1uz << 10, mb = 1uz << 20;

    std::vector<void *> vector(kb, nullptr);

    for (auto _: state) {
        for (auto i = 0uz; i < kb; ++i) vector[i] = operator new(mb);
        for (auto i = 0uz; i < kb; i += 2) operator delete(vector[i], mb);
        for (auto i = 0uz; i < kb; i += 2) vector[i] = operator new(mb);
        for (auto i = 0uz; i < kb; ++i) operator delete(vector[i], mb);

        benchmark::DoNotOptimize(vector);
    }
}

static void bench_list(benchmark::State &state) {
    auto kb = 1uz << 10, mb = 1uz << 20, gb = 1uz << 30;

    std::vector<void *> vector(kb, nullptr);

    for (auto _: state) {
        ListAllocator allocator(gb, mb);

        Allocator &a = allocator;

        for (auto i = 0uz; i < kb; ++i) a.allocate();
        for (auto i = 0uz; i < kb; i += 2) a.deallocate(vector[i]);
        for (auto i = 0uz; i < kb; i += 2) vector[i] = a.allocate();
        for (auto i = 0uz; i < kb; ++i) a.deallocate(vector[i]);

        benchmark::DoNotOptimize(vector);
    }
}

BENCHMARK(bench_system);
BENCHMARK(bench_list);

int main(int argc, char **argv) {
    ListAllocator allocator(32, 8);

    Allocator &a = allocator;

    a.show();
    a.allocate();
    a.show();
    auto x = a.allocate();
    a.show();
    auto y = a.allocate();
    a.show();
    a.allocate();
    a.show();
    a.allocate();

    a.show();
    a.deallocate(x);
    a.show();
    a.deallocate(y);

    a.show();
    auto z = a.allocate();
    a.show();

    assert(z == y);

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    benchmark::Shutdown();
}
