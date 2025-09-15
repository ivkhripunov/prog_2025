//
// Created by ivankhripunov on 15.09.2025.
//

#include <iostream>
#include <limits>
#include <iomanip>

int main() {
    double res = 1;
    double part = 1;
    double n = 1;
    constexpr double eps = std::numeric_limits<double>::epsilon() * 100;

    while (part > eps) {
        part /= n;
        res += part;
        n++;
    }

    std::cout << std::fixed<< std::setprecision(16) << res << std::endl;
}
