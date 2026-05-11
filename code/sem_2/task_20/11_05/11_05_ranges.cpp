//
// Created by ivan on 11.05.2026.
//

#include <gtest/gtest.h>
#include <algorithm>
#include <ranges>
#include <random>
#include <vector>

TEST(Algorithms, RANGES_REPLACE) {
    std::vector<int> v = {1, 2, 3, 2, 1};
    std::ranges::replace(v, 2, 99);
    EXPECT_EQ(v, (std::vector<int>{1, 99, 3, 99, 1}));
}

TEST(Algorithms, RANGES_FILL) {
    std::vector<int> v(5);
    std::ranges::fill(v, 7);
    EXPECT_EQ(v, (std::vector<int>{7, 7, 7, 7, 7}));
}

TEST(Algorithms, RANGES_UNIQUE) {
    std::vector<int> v = {1, 1, 2, 3, 3, 3, 4};
    const auto [first, last] = std::ranges::unique(v);
    v.erase(first, last);
    EXPECT_EQ(v, (std::vector<int>{1, 2, 3, 4}));
}

TEST(Algorithms, RANGES_ROTATE) {
    std::vector<int> v = {1, 2, 3, 4, 5};
    std::ranges::rotate(v, v.begin() + 2);
    EXPECT_EQ(v, (std::vector<int>{3, 4, 5, 1, 2}));
}

TEST(Algorithms, RANGES_SAMPLE) {
    const std::vector<int> source = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> result;
    std::mt19937 rng(42);
    std::ranges::sample(source, std::back_inserter(result), 4, rng);
    EXPECT_EQ(result.size(), 4u);
    for (const auto x: result) {
        EXPECT_TRUE(std::ranges::find(source, x) != source.end());
    }
}
