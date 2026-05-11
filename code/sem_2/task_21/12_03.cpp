//
// Created by ivan on 11.05.2026.
//

#include <gtest/gtest.h>
#include <string>
#include <string_view>
#include <vector>

[[nodiscard]] std::string longest_palindrome(const std::string_view s) {
    const std::size_t n = s.size();

    if (n == 0) {
        return {};
    }

    std::vector<bool> dp(n * n, false);

    const auto at = [&](const std::size_t i, const std::size_t j) -> std::vector<bool>::reference {
        return dp[i * n + j];
    };

    std::size_t best_start = 0;
    std::size_t best_length = 1;

    for (std::size_t i = 0; i < n; ++i) {
        at(i, i) = true;
    }

    for (std::size_t i = 0; i + 1 < n; ++i) {
        if (s[i] == s[i + 1]) {
            at(i, i + 1) = true;
            if (best_length < 2) {
                best_start = i;
                best_length = 2;
            }
        }
    }

    for (std::size_t len = 3; len <= n; ++len) {
        for (std::size_t i = 0; i + len <= n; ++i) {
            const std::size_t j = i + len - 1;
            if (s[i] == s[j] && at(i + 1, j - 1)) {
                at(i, j) = true;
                if (len > best_length) {
                    best_start = i;
                    best_length = len;
                }
            }
        }
    }

    return std::string(s.substr(best_start, best_length));
}

TEST(LONGEST_PALINDROME, EMPTY) {
    ASSERT_EQ(longest_palindrome(""), "");
}

TEST(LONGEST_PALINDROME, ONE_LETTER_STRING) {
    ASSERT_EQ(longest_palindrome("s"), "s");
}

TEST(LONGEST_PALINDROME, ONE_LETTER) {
    ASSERT_EQ(longest_palindrome("abcd").size(), 1u);
}

TEST(LONGEST_PALINDROME, IN_MIDDLE) {
    ASSERT_EQ(longest_palindrome("sahararahnide"), "hararah");
}

TEST(LONGEST_PALINDROME, FULL_STRING) {
    ASSERT_EQ(longest_palindrome("level"), "level");
}
