//
// Created by khripunov on 13.09.2025.
//

#include <iostream>
#include <cmath>
#include <array>
#include <limits>

int main()
{
    double a, b, c;
    std::cin >> a >> b >> c;

    constexpr double eps = std::numeric_limits<double>::epsilon() * 100;
    const bool isLinear = std::abs(a) < eps;

    if (isLinear)
    {
        if (std::abs(b) < eps)
        {
            if (std::abs(c) < eps)
            {
                std::cout << "Бесконечное множество решений" << std::endl;
            }
            else
            {
                std::cout << "Нет решений" << std::endl;
            }
        }
        else
        {
            const double x = -c / b;
            std::cout << "Линейное уравнение, один корень: x = " << x << std::endl;
        }

        return 0;
    }

    const double Q1 = b / a / 2;
    const double Q2 = Q1 * Q1;
    const double D = Q2 - c / a;

    if (std::abs(D) < eps)
    {
        const double root = -Q1;
        std::cout << "Один корень (кратности 2): x = " << root << std::endl;
    }

    else if (D > 0)
    {
        const double sqrtD = std::sqrt(D);
        const double root_1 = -Q1 + sqrtD;
        const double root_2 = -Q1 - sqrtD;
        std::cout << "Два действительных корня: x1 = " << root_1 << ", x2 = " << root_2 << std::endl;
    }
    else
    {
        std::cout << "Нет действительных корней";
    }

    return 0;
}
