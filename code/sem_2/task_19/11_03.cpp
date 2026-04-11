#include <algorithm>
#include <functional>
#include <iterator>
#include <numeric>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

template<typename T, typename Compare>
[[nodiscard]] T median_of_three(const T &a, const T &b, const T &c, const Compare &cmp) {
    const auto lo = cmp(a, b) ? a : b;
    const auto hi = cmp(a, b) ? b : a;

    if (cmp(c, lo)) return lo;
    if (cmp(hi, c)) return hi;

    return c;
}

template<typename Iterator, typename Compare>
void order(const Iterator first, const Iterator last, const Compare &cmp) {
    for (auto i = std::next(first); i != last; ++i)
        for (auto j = i; j != first; --j)
            if (cmp(*j, *std::prev(j)))
                std::iter_swap(j, std::prev(j));
}

template<typename Iterator, typename Compare>
void qsort(const Iterator first, const Iterator last, Compare cmp) {
    if (std::distance(first, last) > 16) {
        const auto middle = std::next(first, std::distance(first, last) / 2);
        const auto pivot = median_of_three(*first, *middle, *std::prev(last), cmp);

        auto i = first;
        auto j = std::prev(last);

        while (i <= j) {
            while (cmp(*i, pivot)) ++i;
            while (cmp(pivot, *j)) --j;

            if (i <= j) {
                if (i != j) std::iter_swap(i, j);
                ++i;
                --j;
            }
        }

        if (first < j) qsort(first, std::next(j), cmp);
        if (i < last) qsort(i, last, cmp);
    } else {
        order(first, last, cmp);
    }
}

template<typename Iterator>
void qsort(const Iterator first, const Iterator last) {
    using value_t = std::iterator_traits<Iterator>::value_type;

    qsort(first, last, std::less<value_t>{});
}

template<typename T>
bool ascending(const T &a, const T &b) { return a < b; }

template<typename T>
bool descending(const T &a, const T &b) { return a > b; }

TEST(TASK_09_04, ascending) {
    constexpr auto size = 1'000uz;
    std::vector<int> v(size);
    for (auto i = 0uz; i < size; ++i) v[i] = size - i;

    qsort(v.begin(), v.end(), ascending<int>);

    ASSERT_TRUE(std::ranges::is_sorted(v, ascending<int>));
}

TEST(TASK_09_04, descending) {
    constexpr auto size = 1'000uz;
    std::vector<int> v(size);
    for (auto i = 0uz; i < size; ++i) v[i] = static_cast<int>(i);

    qsort(v.begin(), v.end(), descending<int>);

    ASSERT_TRUE(std::ranges::is_sorted(v, descending<int>));
}

TEST(TASK_09_04, less) {
    constexpr auto size = 1'000uz;
    std::vector<int> v(size);
    for (auto i = 0uz; i < size; ++i) v[i] = size - i;

    qsort(v.begin(), v.end(), std::less<int>{});

    ASSERT_TRUE(std::ranges::is_sorted(v, std::less<int>{}));
}

TEST(TASK_09_04, greater) {
    constexpr auto size = 1'000uz;
    std::vector<int> v(size);
    for (auto i = 0uz; i < size; ++i) v[i] = static_cast<int>(i);

    qsort(v.begin(), v.end(), std::greater<int>{});

    ASSERT_TRUE(std::ranges::is_sorted(v, std::greater<int>{}));
}

TEST(TASK_09_04, lambda) {
    constexpr auto size = 1'000uz;
    std::vector<int> v(size);
    for (auto i = 0uz; i < size; ++i) v[i] = size - i;

    auto cmp = [](const int &a, const int &b) { return a < b; };

    qsort(v.begin(), v.end(), cmp);

    ASSERT_TRUE(std::ranges::is_sorted(v, cmp));
}
