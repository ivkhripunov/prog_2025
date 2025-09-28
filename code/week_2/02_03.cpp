//
// Created by khripunov on 13.09.2025.
//

#include <iostream>

int main() {
    char ch;
    std::cout << "Введите символ: ";
    std::cin >> ch;

//    const auto code = static_cast<uint>(ch);
//    std::cout << "Символ '" << ch << "' (код " << code << ") принадлежит к классу: ";

    switch (ch) {
        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'F':
        case 'G':
        case 'H':
        case 'I':
        case 'J':
        case 'K':
        case 'L':
        case 'M':
        case 'N':
        case 'O':
        case 'P':
        case 'Q':
        case 'R':
        case 'S':
        case 'T':
        case 'U':
        case 'V':
        case 'W':
        case 'X':
        case 'Y':
        case 'Z':
            std::cout << "заглавные буквы";
            break;

        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
        case 'g':
        case 'h':
        case 'i':
        case 'j':
        case 'k':
        case 'l':
        case 'm':
        case 'n':
        case 'o':
        case 'p':
        case 'q':
        case 'r':
        case 's':
        case 't':
        case 'u':
        case 'v':
        case 'w':
        case 'x':
        case 'y':
        case 'z':
            std::cout << "строчные буквы";
            break;

        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            std::cout << "десятичные цифры";
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
            std::cout << "прочие символы";
            break;
    }

    std::cout << std::endl;
    return 0;
}
