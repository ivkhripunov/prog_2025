///////////////////////////////////////////////////////////////////////////////////

// chapter : Memory Management

///////////////////////////////////////////////////////////////////////////////////

// section : Iterator Classification

///////////////////////////////////////////////////////////////////////////////////

// content : List Iterators
//
// content : Operator ->
//
// content : Range-Based Statement for

///////////////////////////////////////////////////////////////////////////////////

#include <iterator>
#include <memory>
#include <iostream>

///////////////////////////////////////////////////////////////////////////////////

template<typename T>
class List {
    struct Node {
        T x = T();

        std::shared_ptr<Node> next;
        std::weak_ptr<Node> prev;
    };

public :
    class Iterator {
    public :
        using iterator_category = std::forward_iterator_tag;

        //  -------------------------------------------------------------------

        Iterator(std::shared_ptr<Node> node,
                 std::shared_ptr<Node> prev_end)
            : m_node(std::move(node)), m_prev_end(std::move(prev_end)) {
        }

        //  -------------------------------------------------------------------

        auto const operator++(int) {
            auto x = *this;

            m_node = m_node->next;

            return x;
        }

        //  -------------------------------------------------------------------

        auto &operator++() {
            m_node = m_node->next;

            return *this;
        }

        //  -------------------------------------------------------------------

        auto const operator--(int) {
            auto x = *this;
            m_node = m_node ? m_node->prev.lock() : m_prev_end;
            return x;
        }

        //  -------------------------------------------------------------------

        auto &operator--() {
            m_node = m_node ? m_node->prev.lock() : m_prev_end;
            return *this;
        }

        //  -------------------------------------------------------------------

        auto &operator*() const { return m_node->x; }

        auto operator->() const { return &m_node->x; }

        //  -------------------------------------------------------------------

        friend auto operator==(Iterator const &lhs, Iterator const &rhs) {
            return lhs.m_node == rhs.m_node;
        }

    private :
        std::shared_ptr<Node> m_node;
        std::shared_ptr<Node> m_prev_end;
    };

    //  -----------------------------------------------------------------------

    auto begin() const { return Iterator(m_head, m_tail); }

    auto end() const { return Iterator(nullptr, m_tail); }

    //  -----------------------------------------------------------------------

    void push_back(T x) {
        auto node = std::make_shared<Node>(Node{x, nullptr, {}});

        if (m_tail) {
            node->prev = m_tail;
            m_tail->next = node;
            m_tail = node;
        } else {
            m_head = node;
            m_tail = node;
        }
    }

private :
    std::shared_ptr<Node> m_head;
    std::shared_ptr<Node> m_tail;
};

///////////////////////////////////////////////////////////////////////////////////

int main() {
    List<int> list;

    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    list.push_back(4);
    list.push_back(5);

    std::cout << "forward: ";
    for (auto it = std::begin(list); it != std::end(list); ++it) {
        std::cout << *it << ' ';
    }
    std::cout << '\n';

    std::cout << "range-for: ";
    for ([[maybe_unused]] auto element: list) {
        std::cout << element << ' ';
    }
    std::cout << '\n';

    std::cout << "backward: ";
    auto it = std::end(list);
    while (it != std::begin(list)) {
        --it;
        std::cout << *it << ' ';
    }
}

///////////////////////////////////////////////////////////////////////////////////
