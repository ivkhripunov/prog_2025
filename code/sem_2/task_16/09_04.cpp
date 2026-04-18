////////////////////////////////////////////////////////////////////////////////////

// support : www.cs.usfca.edu/~galles/visualization/ComparisonSort.html

////////////////////////////////////////////////////////////////////////////////////

#include <algorithm>
#include <iterator>
#include <numeric>
#include <utility>
#include <vector>
#include <gtest/gtest.h>

template<typename T>
[[nodiscard]] T median_of_three(const T &a, const T &b, const T &c) {
    return std::clamp(a, std::min(b, c), std::max(b, c));
}

////////////////////////////////////////////////////////////////////////////////////

template<typename Iterator>
void order(const Iterator first, const Iterator last) {
    for (auto i = std::next(first); i != last; ++i) {
        for (auto j = i; j != first; --j) {
            if (*std::prev(j) > *j) {
                std::iter_swap(j, std::prev(j));
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////

template<typename Iterator>
void qsort(const Iterator first, const Iterator last) {
    if (std::distance(first, last) > 16) {
        const auto middle = std::next(first, std::distance(first, last) / 2);
        const auto pivot = median_of_three(*first, *middle, *std::prev(last));

        auto i = first;
        auto j = std::prev(last);

        while (i <= j) {
            while (*i < pivot) ++i;
            while (*j > pivot) --j;

            if (i <= j) {
                if (i != j) {
                    std::iter_swap(i, j);
                }
                ++i;
                --j;
            }
        }

        if (first < j) qsort(first, std::next(j));
        if (i < last) qsort(i, last);
    } else {
        order(first, last);
    }
}

TEST(TASK_09_04, QSORT_ITER) {
    auto size = 1'000uz;

    //  ---------------------------------------

    std::vector vector(size, 0);

    //  ---------------------------------------

    for (auto i = 0uz; i < size; ++i) {
        vector[i] = size - i;
    }

    //  ---------------------------------------

    qsort(vector.begin(), vector.end());

    //  ---------------------------------------

    ASSERT_TRUE(std::ranges::is_sorted(vector));
}

////////////////////////////////////////////////////////////////////////////////////
// Score is 10/10
