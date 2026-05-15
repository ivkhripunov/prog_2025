//
// Created by ivan on 15.05.2026.
//

#include <gtest/gtest.h>
#include <cctype>
#include <cmath>
#include <iostream>
#include <print>
#include <sstream>
#include <string>
#include <unordered_map>
#include <variant>

class Stream {
public:
    using token_t = std::variant<char, double, std::string>;

    Stream(const std::string &string) : m_stream(string + ';') {
    }

    auto empty() { return m_stream.peek() == ';'; }

    auto get() {
        if (m_has_token) {
            m_has_token = false;
            return m_token;
        }

        auto x = '\0';
        m_stream >> x;

        switch (x) {
            case '+':
            case '-':
            case '*':
            case '/':
            case '%':
            case '^':
            case '!':
            case '(':
            case ')':
            case '[':
            case ']':
            case '{':
            case '}':
            case ';':
                return token_t(x);

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
            case '.':
                m_stream.unget();
                double y;
                m_stream >> y;
                return token_t(y);

            default: {
                std::string s(1, x);
                while (m_stream.get(x) && (std::isalpha(x) || std::isdigit(x)))
                    s += x;
                if (!std::isspace(x)) m_stream.unget();
                return token_t(s);
            }
        }
    }

    void put(const token_t &token) {
        m_token = token;
        m_has_token = true;
    }

private:
    std::stringstream m_stream;
    token_t m_token;
    bool m_has_token = false;
};

class Calculator {
public:
    void test() {
        std::string string;

        std::print("Calculator::test : enter std::string string(s) : \n");

        while (std::getline(std::cin >> std::ws, string, '\n')) {
            if (Stream stream(string); !stream.empty()) {
                std::print("Calculator::test : {} = {}\n", string, statement(stream));
            } else {
                break;
            }
        }
    }

    auto eval(const std::string &s) -> double {
        Stream stream(s);
        return statement(stream);
    }

private:
    static auto factorial(int n) -> double {
        auto result = 1.0;
        for (auto i = 2; i <= n; ++i) result *= i;
        return result;
    }

    auto statement(Stream &stream) -> double {
        auto token = stream.get();

        if (std::holds_alternative<std::string>(token)) {
            if (std::get<std::string>(token) == "set") {
                return declaration(stream);
            }
        }

        stream.put(token);

        return expression(stream);
    }

    auto declaration(Stream &stream) -> double {
        auto string = std::get<std::string>(stream.get());

        m_variables[string] = expression(stream);

        return m_variables[string];
    }

    auto expression(Stream &stream) const -> double {
        auto x = term(stream);
        while (true) {
            auto token = stream.get();
            if (!std::holds_alternative<char>(token)) {
                stream.put(token);
                return x;
            }
            switch (std::get<char>(token)) {
                case '+': x += term(stream);
                    break;
                case '-': x -= term(stream);
                    break;
                default: stream.put(token);
                    return x;
            }
        }
    }

    auto term(Stream &stream) const -> double {
        auto x = power(stream);
        while (true) {
            auto token = stream.get();
            if (!std::holds_alternative<char>(token)) {
                stream.put(token);
                return x;
            }
            switch (std::get<char>(token)) {
                case '*': x *= power(stream);
                    break;
                case '/': x /= power(stream);
                    break;
                case '%': x = std::fmod(x, power(stream));
                    break;
                default: stream.put(token);
                    return x;
            }
        }
    }

    auto power(Stream &stream) const -> double {
        auto x = postfix(stream);
        auto token = stream.get();
        if (std::holds_alternative<char>(token) && std::get<char>(token) == '^')
            return std::pow(x, power(stream));
        stream.put(token);
        return x;
    }

    auto postfix(Stream &stream) const -> double {
        auto x = primary(stream);
        while (true) {
            auto token = stream.get();
            if (std::holds_alternative<char>(token) && std::get<char>(token) == '!')
                x = factorial(static_cast<int>(x));
            else {
                stream.put(token);
                return x;
            }
        }
    }

    auto primary(Stream &stream) const -> double {
        auto token = stream.get();
        if (std::holds_alternative<char>(token)) {
            switch (std::get<char>(token)) {
                case '(':
                case '[':
                case '{': {
                    auto x = expression(stream);
                    stream.get();
                    return x;
                }
                case '+': return primary(stream);
                case '-': return -primary(stream);
            }
        }
        if (std::holds_alternative<double>(token))
            return std::get<double>(token);
        return m_variables.at(std::get<std::string>(token));
    }

    std::unordered_map<std::string, double> m_variables;
};

TEST(CALCULATOR, MODULO) {
    ASSERT_DOUBLE_EQ(Calculator().eval("10 % 3"), 1.0);
}

TEST(CALCULATOR, POWER) {
    ASSERT_DOUBLE_EQ(Calculator().eval("2 ^ 10"), 1024.0);
}

TEST(CALCULATOR, FACTORIAL) {
    ASSERT_DOUBLE_EQ(Calculator().eval("5!"), 120.0);
}

TEST(CALCULATOR, SQUARE_BRACKETS) {
    ASSERT_DOUBLE_EQ(Calculator().eval("[2 + 3] * 4"), 20.0);
}

TEST(CALCULATOR, CURLY_BRACKETS) {
    ASSERT_DOUBLE_EQ(Calculator().eval("{4 - 1} * 3"), 9.0);
}
