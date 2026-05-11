//
// Created by ivan on 11.05.2026.
//

#include <gtest/gtest.h>
#include <cmath>
#include <numeric>
#include <ranges>
#include <vector>

template<std::ranges::input_range R1, std::ranges::input_range R2>
[[nodiscard]] double mae(R1 &&actual, R2 &&predicted) {
    const auto diff = std::views::zip(actual, predicted)
                      | std::views::transform([](const auto &pair) {
                          return std::abs(std::get<0>(pair) - std::get<1>(pair));
                      });
    const double sum = std::accumulate(std::ranges::begin(diff),
                                       std::ranges::end(diff), 0.0);
    return sum / static_cast<double>(std::ranges::distance(diff));
}

template<std::ranges::input_range R1, std::ranges::input_range R2>
[[nodiscard]] double mse(R1 &&actual, R2 &&predicted) {
    const auto diff_sq = std::views::zip(actual, predicted)
                         | std::views::transform([](const auto &pair) {
                             const double d = std::get<0>(pair) - std::get<1>(pair);
                             return d * d;
                         });
    const double sum = std::accumulate(std::ranges::begin(diff_sq),
                                       std::ranges::end(diff_sq), 0.0);
    return sum / static_cast<double>(std::ranges::distance(diff_sq));
}

TEST(MAE, BASIC) {
    const std::vector<double> actual = {1.0, 2.0, 3.0};
    const std::vector<double> predicted = {1.5, 2.5, 2.0};
    ASSERT_DOUBLE_EQ(mae(actual, predicted), (0.5 + 0.5 + 1.0) / 3.0);
}

TEST(MSE, BASIC) {
    const std::vector<double> actual = {1.0, 2.0, 3.0};
    const std::vector<double> predicted = {1.5, 2.5, 2.0};
    ASSERT_DOUBLE_EQ(mse(actual, predicted), (0.25 + 0.25 + 1.0) / 3.0);
}
