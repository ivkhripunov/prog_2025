//
// Created by ivankhripunov on 28.09.2025.
//

#include <iostream>
#include <vector>

int main() {
    constexpr std::size_t max_number = 100;
    std::vector<std::size_t> cache;
    cache.reserve(max_number);

    std::size_t max_len = 0;
    std::size_t max_len_init = 0;
    for (std::size_t init_number = 1; init_number <= max_number; ++init_number) {
        unsigned long long int curr_number = init_number;
        std::size_t len = 1;

        while (curr_number != 1) {
            if (curr_number < cache.size()) {
                len += cache[curr_number - 1] - 1;
                break;
            }

            curr_number = (curr_number % 2 == 0) ? curr_number / 2 : curr_number * 3 + 1;
            len++;
        }

        cache.push_back(len);

        if (len > max_len) {
            max_len = len;
            max_len_init = init_number;
        }
    }

    std::cout << "Max Kollatz length: " << max_len << std::endl;
    std::cout << "Initial number: " << max_len_init << std::endl;
}
