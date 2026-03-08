//
// Created by ivan on 08.03.2026.
//

#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <algorithm>

static const std::string TARGET = "methinksitislikeaweasel";
static const int COPIES = 100;
static const double MUT_RATE = 0.05;

int divergence(const std::string &s) {
    int diff = 0;
    for (size_t i = 0; i < s.size(); ++i)
        if (s[i] != TARGET[i]) ++diff;
    return diff;
}

int main() {
    const size_t LEN = TARGET.size();

    std::random_device rd;
    std::default_random_engine rng(rd());
    std::uniform_int_distribution<> letter(0, 25);
    std::uniform_real_distribution<> chance(0.0, 1.0);

    std::string current(LEN, ' ');
    for (char &c: current)
        c = static_cast<char>('a' + letter(rng));

    std::cout << "Target : " << TARGET << "\n";
    std::cout << "Start  : " << current << "  divergence=" << divergence(current) << "\n\n";

    for (int generation = 1; ; ++generation) {
        std::vector<std::string> offspring(COPIES);
        for (std::string &child: offspring) {
            child = current;
            for (char &c: child)
                if (chance(rng) < MUT_RATE)
                    c = static_cast<char>('a' + letter(rng));
        }

        const std::string &best = *std::min_element(
            offspring.begin(), offspring.end(),
            [](const std::string &a, const std::string &b) {
                return divergence(a) < divergence(b);
            });

        int bestDiv = divergence(best);
        std::cout << "Gen " << generation << ": " << best << "  divergence=" << bestDiv << "\n";

        if (bestDiv == 0) break;

        current = best;
    }

    std::cout << "\nReached target: " << TARGET << "\n";
    return 0;
}
