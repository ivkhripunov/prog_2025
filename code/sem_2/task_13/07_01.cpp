//
// Created by khripunov on 13.09.2025.
//

#include <iostream>
#include <cmath>
#include <limits>
#include <optional>
#include <variant>
#include <utility>

using OneRootType = double;
using TwoRootsType = std::pair<double, double>;
using InfRootsType = std::monostate;

using Variant = std::variant<OneRootType, TwoRootsType, InfRootsType>;

[[nodiscard]] std::optional<Variant> solve(const double a, const double b, const double c) {
    constexpr double eps = std::numeric_limits<double>::epsilon() * 100;
    const bool isLinear = std::abs(a) < eps;

    if (isLinear) {
        if (std::abs(b) < eps) {
            if (std::abs(c) < eps) {
                return Variant(std::monostate{});
            }

            return std::nullopt;
        }

        const double x = -c / b;
        return Variant(x);
    }

    const double Q1 = b / a / 2;
    const double Q2 = Q1 * Q1;
    const double D = Q2 - c / a;

    if (std::abs(D) < eps) {
        const double root = -Q1;
        return Variant(root);
    }

    if (D > 0) {
        const double sqrtD = std::sqrt(D);
        const double root_1 = -Q1 + sqrtD;
        const double root_2 = -Q1 - sqrtD;
        return Variant(std::make_pair(root_1, root_2));
    }

    return std::nullopt;
}

int main() {
    auto test = [](double a, double b, double c) {
        auto result = solve(a, b, c);
        if (!result.has_value()) std::cout << "Нет решений\n";
        else if (std::holds_alternative<std::monostate>(*result)) std::cout << "Бесконечно много\n";
        else if (std::holds_alternative<double>(*result)) std::cout << "Один корень: " << std::get<double>(*result) << "\n";
        else {
            auto [x1, x2] = std::get<std::pair<double, double>>(*result);
            std::cout << "Два корня: " << x1 << " " << x2 << "\n";
        }
    };

    test(0, 0, 0);
    test(1, -2, 1);
    test(1, -3, 2);
    test(0, 0, 1);

    return 0;
}