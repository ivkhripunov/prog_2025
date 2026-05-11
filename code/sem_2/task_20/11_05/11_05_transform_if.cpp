//
// Created by ivan on 11.05.2026.
//

#include <gtest/gtest.h>
#include <algorithm>
#include <ranges>
#include <vector>

template<std::ranges::forward_range R, std::weakly_incrementable O, typename F, typename P>
auto transform_if(R &&range, O out, const F &transform, const P &predicate) {
    return std::ranges::transform(range | std::views::filter(predicate), out, transform);
}

TEST(TRANSFORM_IF, EVEN_SQUARED) {
    const std::vector<int> input = {1, 2, 3, 4, 5, 6};
    std::vector<int> output;
    transform_if(
        input,
        std::back_inserter(output),
        [](const int x) { return x * x; },
        [](const int x) { return x < 3; }
    );
    ASSERT_EQ(output, (std::vector<int>{1, 4}));
}

TEST(TRANSFORM_IF, EMPTY_INPUT) {
    const std::vector<int> input = {};
    std::vector<int> output;
    transform_if(
        input,
        std::back_inserter(output),
        [](const int x) { return x * x; },
        [](const int x) { return x < 3; }
    );
    ASSERT_TRUE(output.empty());
}

TEST(TRANSFORM_IF, NO_MATCHES) {
    const std::vector<int> input = {1, 3, 5};
    std::vector<int> output;
    transform_if(
        input,
        std::back_inserter(output),
        [](const int x) { return x * x; },
        [](const int x) { return x > 5; }
    );
    ASSERT_TRUE(output.empty());
}
