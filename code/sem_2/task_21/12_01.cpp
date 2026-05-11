//
// Created by ivan on 11.05.2026.
//

#include <iomanip>
#include <iostream>
#include <locale>
#include <sstream>
#include <string>

int main() {
    constexpr long double rate = 1.0L / 90.0L;

    const std::locale ru("ru_RU.UTF-8");
    const std::locale en("en_US.UTF-8");

    std::cout << "Введите сумму в рублях (например, 990,99): ";
    std::string line;
    std::getline(std::cin, line);
    line += " ₽";

    std::istringstream iss(line);
    iss.imbue(ru);
    long double rub = 0;
    iss >> std::get_money(rub);

    const long double usd = rub * rate;

    std::ostringstream rub_oss;
    rub_oss.imbue(ru);
    rub_oss << std::showbase << std::put_money(rub);

    std::ostringstream usd_oss;
    usd_oss.imbue(en);
    usd_oss << std::showbase << std::put_money(usd);

    std::cout << "RUB: " << rub_oss.str() << "\n";
    std::cout << "USD: " << usd_oss.str() << "\n";
}
