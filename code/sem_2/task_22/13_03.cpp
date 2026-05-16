//
// Created by ivan on 15.05.2026.
//

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <iterator>
#include <fstream>
#include <print>
#include <ranges>
#include <sstream>
#include <string>

void transform(std::string const &path_1, std::string const &path_2) {
    auto string = (std::stringstream() << std::fstream(path_1, std::ios::in).rdbuf()).str();

    for (auto iterator = std::begin(string); iterator != std::end(string); ++iterator) {
        if (*iterator == 'R' && *std::next(iterator) == '"') {
            auto cur = std::next(iterator, 2);
            std::string close = ")";
            while (*cur != '(') close += *cur++;
            close += '"';
            ++cur;
            while (string.compare(std::distance(std::begin(string), cur), close.size(), close) != 0)
                ++cur;
            iterator = std::next(cur, close.size() - 1);
            continue;
        }

        if (*iterator == '\'') {
            do { ++iterator; } while (!(*iterator == '\'' && *std::prev(iterator) != '\\'));
        }

        if (*iterator == '\"') {
            do { ++iterator; } while (!(*iterator == '\"' && *std::prev(iterator) != '\\'));
        }

        if (*iterator == '/') {
            if (*std::next(iterator) == '/') {
                auto end = std::next(iterator, 2);
                while (end != std::end(string) && *end != '\n') { ++end; }
                iterator = string.erase(iterator, end);
            } else if (*std::next(iterator) == '*') {
                auto end = std::next(iterator, 3);
                while (!(*end == '/' && *std::prev(end) == '*')) { ++end; }
                iterator = string.erase(iterator, ++end);
            }
        }

        if (iterator == std::end(string)) { break; }
    }

    std::istringstream iss(string);
    std::ostringstream oss;
    std::string line;
    while (std::getline(iss, line)) {
        if (!std::ranges::all_of(line, [](unsigned char c) { return std::isspace(c); }))
            oss << line << '\n';
    }

    std::fstream(path_2, std::ios::out) << oss.str();
}

int main() {
    auto path_1 = "source.cpp";
    auto path_2 = "output.cpp";

    std::fstream(path_1, std::ios::out) <<
            "// коммент\n"
            "\n"
            "#include <iostream>\n"
            "\n"
            "/* многонстрочный\n"
            "   коммент */\n"
            "int main() {\n"
            "    auto s = R\"(// строка /* строка */)\"; // коммент\n"
            "    return 0; // коммент\n"
            "}\n";

    transform(path_1, path_2);

    std::print("{}", (std::stringstream() << std::fstream(path_2, std::ios::in).rdbuf()).str());

    std::print("main : enter char : ");
    std::cin.get();

    std::filesystem::remove(path_1);
    std::filesystem::remove(path_2);
}
