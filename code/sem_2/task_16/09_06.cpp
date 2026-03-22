#include <boost/iterator/iterator_facade.hpp>
#include <iostream>
#include <iterator>

class Iterator {
public:
    using iterator_category = std::forward_iterator_tag;

    explicit Iterator(int) : m_a(0), m_b(1) {
    }

    Iterator operator++(int) {
        const auto x = *this;
        advance();
        return x;
    }

    Iterator &operator++() {
        advance();
        return *this;
    }

    int operator*() const { return m_a; }

    friend bool operator==(const Iterator &lhs, const Iterator &rhs) {
        return lhs.m_a == rhs.m_a && lhs.m_b == rhs.m_b;
    }

private:
    void advance() {
        const auto next = m_a + m_b;
        m_a = m_b;
        m_b = next;
    }

    int m_a;
    int m_b;
};

class BoostIterator
        : public boost::iterator_facade<
            BoostIterator,
            const int,
            boost::forward_traversal_tag> {
public:
    explicit BoostIterator(int) : m_a(0), m_b(1) {
    }

private:
    friend class boost::iterator_core_access;

    void increment() {
        const auto next = m_a + m_b;
        m_a = m_b;
        m_b = next;
    }

    [[nodiscard]] const int &dereference() const { return m_a; }

    [[nodiscard]] bool equal(const BoostIterator &other) const {
        return m_a == other.m_a && m_b == other.m_b;
    }

    int m_a;
    int m_b;
};

int main() {
    constexpr auto N = 15;

    std::cout << "v1: ";
    auto it = Iterator(0);
    for (auto i = 0; i < N; ++i, ++it) {
        std::cout << *it << ' ';
    }
    std::cout << '\n';

    std::cout << "v2: ";
    auto bit = BoostIterator(0);
    for (auto i = 0; i < N; ++i, ++bit) {
        std::cout << *bit << ' ';
    }
    std::cout << '\n';
}
