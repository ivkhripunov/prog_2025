//
// Created by khripunov on 13.09.2025.
//

#include <gtest/gtest.h>
#include <cmath>
#include <limits>
#include <optional>
#include <variant>
#include <utility>

using OneRootType = double;
using TwoRootsType = std::pair<double, double>;
using InfRootsType = std::monostate;

using VariantType = std::variant<OneRootType, TwoRootsType, InfRootsType>;

[[nodiscard]] std::optional<VariantType> solve(const double a, const double b, const double c) {
    constexpr double eps = std::numeric_limits<double>::epsilon() * 100;
    const bool isLinear = std::abs(a) < eps;

    if (isLinear) {
        if (std::abs(b) < eps) {
            if (std::abs(c) < eps) {
                return VariantType(InfRootsType{});
            }

            return std::nullopt;
        }

        const double x = -c / b;
        return VariantType(x);
    }

    const double Q1 = b / a / 2;
    const double Q2 = Q1 * Q1;
    const double D = Q2 - c / a;

    if (std::abs(D) < eps) {
        const double root = -Q1;
        return VariantType(root);
    }

    if (D > 0) {
        const double sqrtD = std::sqrt(D);
        const double root_1 = -Q1 + sqrtD;
        const double root_2 = -Q1 - sqrtD;
        return VariantType(std::make_pair(root_1, root_2));
    }

    return std::nullopt;
}

TEST(SOLVER, ALL_TESTS) {
    ASSERT_TRUE(std::holds_alternative<InfRootsType>(*solve(0, 0, 0)));
    ASSERT_TRUE(std::holds_alternative<OneRootType>(*solve(1, -2, 1)));
    ASSERT_TRUE(std::holds_alternative<TwoRootsType>(*solve(1, -3, 2)));
    ASSERT_FALSE(solve(0, 0, 1).has_value());
}
