//
// Created by ivan on 08.03.2026.
//

#include <algorithm>
#include <iostream>
#include <random>
#include <ranges>
#include <string>

int main() {
    const std::string TARGET = "methinksitislikeaweasel";
    constexpr std::size_t COPIES = 100;
    constexpr double MUT_RATE = 0.05;

    const auto divergence = [&TARGET](const std::string &s) {
        const auto pred = [](const auto &pair) {
            const auto &[first, second] = pair;
            return first != second;
        };

        return static_cast<std::size_t>(std::ranges::count_if(std::views::zip(s, TARGET), pred));
    };

    std::random_device rd;
    std::default_random_engine rng(rd());
    std::uniform_int_distribution letter(0, 25);
    std::uniform_real_distribution chance(0.0, 1.0);

    std::string current(TARGET.size(), ' ');

    std::ranges::generate(current, [&]() -> char {
        return static_cast<char>('a' + letter(rng));
    });

    std::cout << "Target : " << TARGET << "\n";
    std::cout << "Start  : " << current << "  divergence=" << divergence(current) << "\n\n";

    std::array<std::string, COPIES> offspring;

    const auto update = [&]() -> std::string {
        std::string child = current;
        for (char &c: child)
            if (chance(rng) < MUT_RATE)
                c = static_cast<char>('a' + letter(rng));
        return child;
    };

    const auto cmp = [&divergence](const std::string &a, const std::string &b) -> bool {
        return divergence(a) < divergence(b);
    };

    for (std::size_t generation = 1;; ++generation) {
        std::ranges::generate(offspring, update);

        const auto best_it = std::ranges::min_element(offspring, cmp);
        std::string best = std::move(*best_it);
        const auto best_div = divergence(best);

        std::cout << "Gen " << generation << ": " << best << "  divergence=" << best_div << "\n";

        if (best_div == 0) break;

        current = std::move(best);
    }

    std::cout << "\nReached target: " << TARGET << "\n";
    return 0;
}
