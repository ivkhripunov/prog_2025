//
// Created by khripunov on 13.09.2025.
//

#include <iostream>
#include <cmath>

[[nodiscard]] int calc_fib(const int n)
{
    const double sqrt5 = std::sqrt(5);

    const double first = (1 + sqrt5) / 2;
    const double second = (1 - sqrt5) / 2;

    const double res = (std::pow(first, n) - std::pow(second, n)) / sqrt5;

    return static_cast<int>(res);
}

// результат совпадает с честным вычислением чисел фиб. вплоть до n = 47. При n = 47 значение res превышает max
// значение int, происходит переполнение.

int main()
{
    int n;
    std::cin >> n;
    std::cout << calc_fib(n);
}
