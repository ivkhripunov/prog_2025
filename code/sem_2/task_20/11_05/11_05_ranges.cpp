//
// Created by ivan on 11.05.2026.
//

#include <gtest/gtest.h>
#include <algorithm>
#include <ranges>
#include <random>
#include <vector>

TEST(RANGES, REPLACE) {
    std::vector<int> v = {1, 2, 3, 2, 1};
    std::ranges::replace(v, 2, 99);
    ASSERT_EQ(v, (std::vector<int>{1, 99, 3, 99, 1}));
}

TEST(RANGES, FILL) {
    std::vector<int> v(5);
    std::ranges::fill(v, 7);
    ASSERT_EQ(v, (std::vector<int>{7, 7, 7, 7, 7}));
}

TEST(RANGES, UNIQUE) {
    std::vector<int> v = {1, 1, 2, 3, 3, 3, 4};
    const auto [first, last] = std::ranges::unique(v);
    v.erase(first, last);
    ASSERT_EQ(v, (std::vector<int>{1, 2, 3, 4}));
}

TEST(RANGES, ROTATE) {
    std::vector<int> v = {1, 2, 3, 4, 5};
    std::ranges::rotate(v, v.begin() + 2);
    ASSERT_EQ(v, (std::vector<int>{3, 4, 5, 1, 2}));
}

TEST(RANGES, SAMPLE) {
    const std::vector<int> source = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> result;
    std::mt19937 rng(42);
    std::ranges::sample(source, std::back_inserter(result), 4, rng);
    ASSERT_EQ(result.size(), 4u);
    for (const auto x: result) {
        ASSERT_TRUE(std::ranges::find(source, x) != source.end());
    }
}
