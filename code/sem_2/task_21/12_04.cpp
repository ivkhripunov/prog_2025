//
// Created by ivan on 11.05.2026.
//

#include <gtest/gtest.h>
#include <regex>
#include <string>
#include <vector>
#include <algorithm>

using namespace std::literals;

struct Email {
    std::string email;
    std::string domain;

    bool operator==(const Email &) const = default;
};

[[nodiscard]] std::vector<Email> extract_emails(const std::string &text) {
    const std::regex pattern(R"([a-zA-Z0-9._%+-]+@([a-zA-Z0-9.-]+\.[a-zA-Z]{2,}))");

    std::vector<Email> result;

    std::ranges::for_each(
        std::sregex_iterator(std::cbegin(text), std::cend(text), pattern),
        std::sregex_iterator(),
        [&result](const std::smatch &match) {
            result.push_back({match[0].str(), match[1].str()});
        }
    );

    return result;
}

TEST(EXTRACT_EMAIL, SINGLE) {
    const auto text = R"(Бвамотваолджмтважм xxDDD@example.ru укпуыкииыу.)"s;
    const auto result = extract_emails(text);
    ASSERT_EQ(result.size(), 1u);
    ASSERT_EQ(result[0].email, "xxDDD@example.ru");
    ASSERT_EQ(result[0].domain, "example.ru");
}

TEST(EXTRACT_EMAIL, MULTIPLE) {
    const auto text = R"(
        HWHRTHRTWSJ aaaa@bbbgbg.ru ASRGPAOWGJK^%^^.
        ASOJIA'GJIREEGJEI& VFVRTR@RRRR.ru
    )"s;
    const auto result = extract_emails(text);
    ASSERT_EQ(result.size(), 2u);
    ASSERT_EQ(result[0], (Email{ "aaaa@bbbgbg.ru", "bbbgbg.ru" }));
    ASSERT_EQ(result[1], (Email{ "VFVRTR@RRRR.ru", "RRRR.ru" }));
}

TEST(EXTRACT_EMAIL, NO_EMAILS) {
    const auto text = R"(xlfndoivjiohjeriohert)"s;
    ASSERT_TRUE(extract_emails(text).empty());
}
