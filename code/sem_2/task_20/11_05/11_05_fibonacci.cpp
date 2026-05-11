//
// Created by ivan on 11.05.2026.
//

#include <gtest/gtest.h>
#include <algorithm>
#include <ranges>
#include <vector>

class Fibonacci : public std::ranges::view_interface<Fibonacci> {
    class Iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = std::size_t;
        using difference_type = std::ptrdiff_t;

        Iterator() : m_a(0), m_b(1), m_remaining(0) {
        }

        explicit Iterator(const std::size_t count) : m_a(0), m_b(1), m_remaining(count) {
        }

        std::size_t operator*() const { return m_a; }

        Iterator &operator++() {
            const std::size_t next = m_a + m_b;
            m_a = m_b;
            m_b = next;
            --m_remaining;
            return *this;
        }

        Iterator operator++(int) {
            const Iterator copy = *this;
            ++(*this);
            return copy;
        }

        friend bool operator==(const Iterator &lhs, const Iterator &rhs) {
            return lhs.m_remaining == rhs.m_remaining;
        }

    private:
        std::size_t m_a;
        std::size_t m_b;
        std::size_t m_remaining;
    };

    std::size_t m_count;

public:
    explicit Fibonacci(const std::size_t count) : m_count(count) {
    }

    Iterator begin() const { return Iterator(m_count); }
    Iterator end() const { return Iterator(0); }
};

TEST(FIBONACCI, TEN_ELEMENTS) {
    const auto result = Fibonacci(10) | std::ranges::to<std::vector>();
    ASSERT_EQ(result, (std::vector<std::size_t>{0, 1, 1, 2, 3, 5, 8, 13, 21, 34}));
}

TEST(FIBONACCI, NO_ELEMENTS) {
    const auto result = Fibonacci(0) | std::ranges::to<std::vector>();
    ASSERT_TRUE(result.empty());
}
