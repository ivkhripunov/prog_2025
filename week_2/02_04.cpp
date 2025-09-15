//
// Created by ivankhripunov on 15.09.2025.
//

#include <iostream>

int main() {
    for (auto i = 1uz; i <= 9; ++i) {
        const std::size_t i_3 = i * i * i;
        for (auto j = 0uz; j <= 9; ++j) {
            const std::size_t j_3 = j * j * j;
            for (auto k = 0uz; k <= 9; ++k) {
                const std::size_t k_3 = k * k * k;
                const std::size_t number = i * 100 + j * 10 + k;
                if (i_3 + j_3 + k_3 == number) {
                    std::cout << number << std::endl;
                }
            }
        }
    }
}
