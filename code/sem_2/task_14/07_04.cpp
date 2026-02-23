////////////////////////////////////////////////////////////////////////////////////

// support : www.cs.usfca.edu/~galles/visualization/ComparisonSort.html

////////////////////////////////////////////////////////////////////////////////////

#include <algorithm>
#include <numeric>
#include <utility>
#include <gtest/gtest.h>
#include <vector>

template<typename T>
[[nodiscard]] T median_of_three(const T &a, const T &b, const T &c) {
    return std::clamp(a, std::min(b, c), std::max(b, c));
}

////////////////////////////////////////////////////////////////////////////////////

template<typename T>
void order(std::vector<T> &vector, const std::size_t left, const std::size_t right) {
    for (auto i = left + 1; i < right; ++i) {
        for (auto j = i; j > left; --j) {
            if (vector[j - 1] > vector[j]) {
                std::swap(vector[j], vector[j - 1]);
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////

template<typename T>
void split(std::vector<T> &vector, const std::size_t left, const std::size_t right) {
    if (right - left > 16) {
        const auto middle = std::midpoint(left, right);
        const int pivot = median_of_three(vector.front(), vector[middle], vector.back());
        std::size_t i = left;
        std::size_t j = right - 1;

        while (i <= j) {
            while (vector[i] < pivot) ++i;
            while (vector[j] > pivot) --j;

            if (i <= j) {
                if (i != j) {
                    std::swap(vector[i], vector[j]);
                }
                ++i;
                --j;
            }
        }

        if (left < j) split(vector, left, j + 1);
        if (i < right) split(vector, i, right);
    } else {
        order(vector, left, right);
    }
}

////////////////////////////////////////////////////////////////////////////////////

template<typename T>
void sort(std::vector<T> &vector) {
    split(vector, 0, std::size(vector));
}

////////////////////////////////////////////////////////////////////////////////////

TEST(TASK_07_04, QSORT_T) {
    auto size = 1'000uz;

    //  ---------------------------------------

    std::vector vector(size, 0);

    //  ---------------------------------------

    for (auto i = 0uz; i < size; ++i) {
        vector[i] = size - i;
    }

    //  ---------------------------------------

    sort(vector);

    //  ---------------------------------------

    ASSERT_TRUE(std::ranges::is_sorted(vector));
}

////////////////////////////////////////////////////////////////////////////////////
