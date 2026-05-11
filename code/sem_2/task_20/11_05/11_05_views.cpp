//
// Created by ivan on 11.05.2026.
//

#include <gtest/gtest.h>
#include <ranges>
#include <string>
#include <vector>

TEST(VIEWS, FILTER) {
    const std::vector<int> v = {1, 2, 3, 4, 5};
    auto even = v | std::views::filter([](const int x) { return x % 2 == 0; });
    const std::vector<int> result(even.begin(), even.end());
    ASSERT_EQ(result, (std::vector<int>{2, 4}));
}

TEST(VIEWS, DROP) {
    const std::vector<int> v = {1, 2, 3, 4, 5};
    const auto dropped = v | std::views::drop(2);
    const std::vector<int> result(dropped.begin(), dropped.end());
    ASSERT_EQ(result, (std::vector<int>{3, 4, 5}));
}

TEST(VIEWS, JOIN) {
    const std::vector<std::vector<int> > nested = {{1, 2}, {3, 4}, {5}};
    const auto flat = nested | std::views::join;
    const std::vector<int> result(flat.begin(), flat.end());
    ASSERT_EQ(result, (std::vector<int>{1, 2, 3, 4, 5}));
}

TEST(Views, VIEWS_ZIP) {
    const std::vector<int> keys = {1, 2, 3};
    const std::vector<std::string> values = {"a", "b", "c"};

    using PairType = std::tuple<int, std::string>;
    std::vector<PairType> result;
    std::ranges::copy(std::views::zip(keys, values), std::back_inserter(result));

    ASSERT_EQ(result, (std::vector<PairType>{{1, "a"}, {2, "b"}, {3, "c"}}));
}

TEST(VIEWS, STRIDE) {
    const std::vector<int> v = {0, 1, 2, 3, 4, 5};
    const auto strided = v | std::views::stride(3);
    const std::vector<int> result(strided.begin(), strided.end());
    ASSERT_EQ(result, (std::vector<int>{0, 3}));
}
