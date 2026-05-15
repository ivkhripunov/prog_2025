//
// Created by ivan on 15.05.2026.
//

#include <gtest/gtest.h>
#include <cmath>
#include <fstream>
#include <string_view>
#include <vector>
#include <print>

#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>

namespace x3 = boost::spirit::x3;

class Operand : public boost::spirit::x3::variant
        <
            double,

            boost::spirit::x3::forward_ast<struct Sign>,

            boost::spirit::x3::forward_ast<struct List>
        > {
public :
    using base_type::base_type, base_type::operator=;
};


struct Sign {
    char operation = '\0';
    Operand operand;
};

struct Step {
    char operation = '\0';
    Operand operand;
};

struct List {
    Operand head;
    std::vector<Step> steps;
};

BOOST_FUSION_ADAPT_STRUCT(Sign, operation, operand)

BOOST_FUSION_ADAPT_STRUCT(Step, operation, operand)

BOOST_FUSION_ADAPT_STRUCT(List, head, steps)

namespace parser {
    x3::rule<struct rule_1_tag, List> rule_1;

    x3::rule<struct rule_2_tag, List> rule_2;

    x3::rule<struct rule_3_tag, Operand> rule_3;

    x3::rule<struct fact_tag, double> fact_rule;

    auto rule_1_def = rule_2 >> *(
                          x3::char_('+') >> rule_2 |
                          x3::char_('-') >> rule_2
                      );

    auto rule_2_def = rule_3 >> *(
                          x3::char_('*') >> rule_3 |
                          x3::char_('/') >> rule_3 |
                          x3::char_('%') >> rule_3 |
                          x3::char_('^') >> rule_3
                      );

    auto rule_3_def =
            fact_rule |
            x3::char_('+') >> rule_3 |
            x3::char_('-') >> rule_3 |
            x3::double_ |
            '(' >> rule_1 >> ')' |
            '[' >> rule_1 >> ']' |
            '{' >> rule_1 >> '}';

    const auto factorial_sa = [](auto &ctx) {
        const double n = x3::_attr(ctx);
        double r = 1.0;
        for (int i = 2; i <= static_cast<int>(n); ++i) r *= i;
        x3::_val(ctx) = r;
    };

    auto fact_rule_def = (x3::double_ >> x3::omit[x3::char_('!')])[factorial_sa];

    BOOST_SPIRIT_DEFINE(rule_1, rule_2, rule_3, fact_rule)
}

class Calculator {
public:
    auto operator()(double x) const -> double { return x; }

    auto operator()(const Sign &sign) const -> double {
        auto x = boost::apply_visitor(*this, sign.operand);
        return sign.operation == '-' ? -x : x;
    }

    auto operator()(const Step &step, double x) const -> double {
        const auto y = boost::apply_visitor(*this, step.operand);
        switch (step.operation) {
            case '+': return x + y;
            case '-': return x - y;
            case '*': return x * y;
            case '/': return x / y;
            case '%': return std::fmod(x, y);
            case '^': return std::pow(x, y);
        }
        return x;
    }

    auto operator()(List const &list) const -> double {
        auto x = boost::apply_visitor(*this, list.head);

        for (auto const &step: list.steps) {
            x = (*this)(step, x);
        }

        return x;
    }
};

auto parse(std::string_view view) {
    List list;

    boost::spirit::x3::phrase_parse
    (
        std::begin(view), std::end(view), parser::rule_1, boost::spirit::x3::ascii::space, list
    );

    static Calculator calculator;

    return calculator(list);
}

void parse_file(std::fstream &file) {
    std::string string;
    while (std::getline(file >> std::ws, string)) {
        std::print("{} = {}\n", string, parse(string));
    }
}

int main() {
    std::fstream out("test_input.txt", std::ios::out);
    out << "2 + 3\n10 % 3\n2 ^ 10\n5!\n[2 + 3] * 4\n";
    out.close();

    std::fstream file("test_input.txt", std::ios::in);
    parse_file(file);
}
