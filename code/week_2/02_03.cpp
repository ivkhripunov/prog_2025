//
// Created by khripunov on 13.09.2025.
//

#include <iostream>

int main() {
    char ch;
    std::cout << "Enter symbols: ";
    std::cin >> ch;

    switch (ch) {
        case 'A' ... 'Z':
            std::cout << "Upper case letters";
            break;

        case 'a' ... 'z':
            std::cout << "Lower case letters";
            break;

        case '0' ... '9':
            std::cout << "Decimal digits";
            break;

        case '.':
        case ',':
        case ':':
        case ';':
        case '!':
        case '?':
        case '\'':
        case '"':
            std::cout << "знаки препинания";
            break;

        default:
            std::cout << "Other symbols";
            break;
    }

    std::cout << std::endl;
    return 0;
}
