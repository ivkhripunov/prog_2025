//
// Created by ivan on 11.05.2026.
//

#include <gtest/gtest.h>
#include <cstdint>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

[[nodiscard]] std::string bytes_to_hexstr(const std::vector<std::uint8_t> &bytes) {
    std::ostringstream oss;
    oss << std::hex << std::setfill('0');
    for (const auto byte: bytes) {
        oss << std::right << std::setw(2) << static_cast<int>(byte);
    }
    return oss.str();
}

[[nodiscard]] std::uint8_t hexchar_to_int(const char ch) {
    if (ch >= '0' && ch <= '9') return ch - '0';
    if (ch >= 'a' && ch <= 'f') return ch - 'a' + 10;
    throw std::invalid_argument("неверный симвл");
}

[[nodiscard]] std::vector<std::uint8_t> hexstr_to_bytes(const std::string_view str) {
    std::vector<std::uint8_t> result;
    for (std::size_t i = 0; i + 1 < str.size(); i += 2) {
        result.push_back((hexchar_to_int(str[i]) << 4) | hexchar_to_int(str[i + 1]));
    }
    return result;
}

TEST(TO_HEX, BASIC) {
    const std::vector<std::uint8_t> bytes = {0xba, 0xad, 0xf0, 0x0d};
    ASSERT_EQ(bytes_to_hexstr(bytes), "baadf00d");
}

TEST(TO_HEX, LEADING_ZERO) {
    const std::vector<std::uint8_t> bytes = {0x00, 0x0f, 0xff};
    ASSERT_EQ(bytes_to_hexstr(bytes), "000fff");
}

TEST(TO_HEX, EMPTY) {
    ASSERT_EQ(bytes_to_hexstr({}), "");
}

TEST(TO_BYTES, BASIC) {
    const std::vector<std::uint8_t> expected = {0xba, 0xad, 0xf0, 0x0d};
    ASSERT_EQ(hexstr_to_bytes("baadf00d"), expected);
}

TEST(TO_BYTES, LEADING_ZERO) {
    const std::vector<std::uint8_t> expected = {0x00, 0x0f, 0xff};
    ASSERT_EQ(hexstr_to_bytes("000fff"), expected);
}

TEST(TO_BYTES, EMPTY) {
    ASSERT_TRUE(hexstr_to_bytes("").empty());
}
