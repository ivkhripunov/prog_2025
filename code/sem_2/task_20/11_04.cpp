//
// Created by khripunov on 11.05.2026.
//

#include <gtest/gtest.h>
#include <cmath>
#include <limits>
#include <optional>
#include <variant>
#include <utility>
#include <vector>

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

class Visitor {
public:
    void operator()(const OneRootType &root) {
        roots.push_back(root);
    }

    void operator()(const TwoRootsType &pair) {
        roots.push_back(pair.first);
        roots.push_back(pair.second);
    }

    void operator()(const InfRootsType &) {
        inf_roots = true;
    }

    std::vector<double> roots;
    bool inf_roots = false;
};

[[nodiscard]] Visitor extract_roots(const std::optional<VariantType> &opt) {
    Visitor visitor;
    if (opt.has_value()) {
        std::visit(visitor, opt.value());
    }
    return visitor;
}

TEST(SOLVER, NO_REAL_SOLUTION) {
    const auto result = solve(1, 0, 1);
    ASSERT_FALSE(result.has_value());
}

TEST(SOLVER, NO_SOLUTION) {
    const auto result = solve(0, 0, 1);
    ASSERT_FALSE(result.has_value());
}

TEST(SOLVER, INFINITE_ROOTS) {
    const auto result = solve(0, 0, 0);
    ASSERT_TRUE(result.has_value());
    ASSERT_TRUE(std::holds_alternative<InfRootsType>(*result));

    const auto visitor = extract_roots(result);
    ASSERT_TRUE(visitor.inf_roots);
    ASSERT_TRUE(visitor.roots.empty());
}

TEST(SOLVER, ONE_DOUBLE_ROOT) {
    const auto result = solve(1, -2, 1);
    ASSERT_TRUE(result.has_value());
    ASSERT_TRUE(std::holds_alternative<OneRootType>(*result));

    const auto visitor = extract_roots(result);
    ASSERT_FALSE(visitor.inf_roots);
    ASSERT_EQ(visitor.roots.size(), 1u);
    ASSERT_DOUBLE_EQ(visitor.roots[0], 1.0);
}

TEST(SOLVER, ONE_ROOT_LINEAR) {
    const auto result = solve(0, 2, 4);
    ASSERT_TRUE(result.has_value());
    ASSERT_TRUE(std::holds_alternative<OneRootType>(*result));

    const auto visitor = extract_roots(result);
    ASSERT_FALSE(visitor.inf_roots);
    ASSERT_EQ(visitor.roots.size(), 1u);
    ASSERT_DOUBLE_EQ(visitor.roots[0], -2.0);
}

TEST(SOLVER, TWO_ROOTS) {
    const auto result = solve(1, -3, 2);
    ASSERT_TRUE(result.has_value());
    ASSERT_TRUE(std::holds_alternative<TwoRootsType>(*result));

    const auto visitor = extract_roots(result);
    ASSERT_FALSE(visitor.inf_roots);
    ASSERT_EQ(visitor.roots.size(), 2u);
    ASSERT_DOUBLE_EQ(visitor.roots[0], 2.0);
    ASSERT_DOUBLE_EQ(visitor.roots[1], 1.0);
}
