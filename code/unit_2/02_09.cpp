//
// Created by ivankhripunov on 01.11.2025.
//

#include <gtest/gtest.h>
#include <numeric>

[[nodiscard]] int gcd_iterative(int a, int b) {
    a = std::abs(a);
    b = std::abs(b);
    while (b) {
        a %= b;
        std::swap(a, b);
    }
    return a;
}

[[nodiscard]] int gcd_recursive(int a, int b) {
    a = std::abs(a);
    b = std::abs(b);
    return b ? gcd_recursive(b, a % b) : a;
}

[[nodiscard]] int lcm(int a, int b) {
    a = std::abs(a);
    b = std::abs(b);
    return a / gcd_iterative(a, b) * b;
}

TEST(TASK_02_09, GCD_LCM) {
    const std::vector<std::pair<int, int>> test_pairs = {{6,   10},
                                                         {-6,  10},
                                                         {-6,  -10},
                                                         {24,  0},
                                                         {-24, 0}};

    for (const auto &pair: test_pairs) {
        const int gcd_ref = std::gcd(pair.first, pair.second);
        const int lcm_ref = std::lcm(pair.first, pair.second);

        ASSERT_EQ(gcd_iterative(pair.first, pair.second), gcd_ref);
        ASSERT_EQ(gcd_recursive(pair.first, pair.second), gcd_ref);

        ASSERT_EQ(lcm(pair.first, pair.second), lcm_ref);
    }
}