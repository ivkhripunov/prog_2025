//
// Created by ivankhripunov on 28.09.2025.
//

#include <iostream>

int main() {
    std::size_t N;
    std::cin >> N;
    auto* array = new double[N];

    for (std::size_t i = 0; i < N; ++i) {
        std::cin >> array[i];
    }

    double max = array[0];
    double min = array[0];
    double sum = array[0];
    for (std::size_t i = 1; i < N; ++i) {
        max = std::max(array[i], max);
        min = std::min(array[i], min);

        sum += array[i];
    }

    const double avg = sum / static_cast<double>(N);

    double sum_sq_diff = 0;
    for (std::size_t i = 0; i < N; ++i) {
        const double diff = array[i] - avg;
        sum_sq_diff += diff * diff;
    }

    const double mse = sum_sq_diff / static_cast<double>(N);

    delete[] array;

    std::cout << "Минимальное значение: " << min << std::endl;
    std::cout << "Максимальное значение: " << max << std::endl;
    std::cout << "Среднее арифметическое: " << avg << std::endl;
    std::cout << "Стандартное отклонение: " << mse << std::endl;
}