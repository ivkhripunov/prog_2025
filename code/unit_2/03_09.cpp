//
// Created by ivankhripunov on 02.11.2025.
//

#include <iostream>
#include <array>
#include <cstdint>
#include <gtest/gtest.h>

class IPv4 {
private:
    std::array<std::uint8_t, 4> data;

public:
    IPv4() : data{0, 0, 0, 0} {}

    IPv4(const std::uint8_t a, const std::uint8_t b, const std::uint8_t c, const std::uint8_t d) : data{a, b, c, d} {}

    IPv4 &operator++() {
        for (int i = 3; i >= 0; --i) {
            if (data[i] < 255) {
                ++data[i];
                break;
            } else {
                data[i] = 0;
            }
        }
        return *this;
    }

    IPv4 operator++(int) {
        IPv4 temp(*this);
        ++(*this);
        return temp;
    }

    IPv4 &operator--() {
        for (int i = 3; i >= 0; --i) {
            if (data[i] > 0) {
                --data[i];
                break;
            } else {
                data[i] = 255;
            }
        }
        return *this;
    }

    IPv4 operator--(int) {
        IPv4 temp(*this);
        --(*this);
        return temp;
    }

    friend auto operator<=>(const IPv4 &lhs, const IPv4 &rhs) = default;

    friend std::ostream &operator<<(std::ostream &os, const IPv4 &ip) {
        os << static_cast<int>(ip.data[0]) << '.'
           << static_cast<int>(ip.data[1]) << '.'
           << static_cast<int>(ip.data[2]) << '.'
           << static_cast<int>(ip.data[3]);
        return os;
    }

    friend std::istream &operator>>(std::istream &is, IPv4 &ip) {
        int a, b, c, d;
        char dot1, dot2, dot3;

        is >> a >> dot1 >> b >> dot2 >> c >> dot3 >> d;

        ip.data[0] = static_cast<std::uint8_t>(a);
        ip.data[1] = static_cast<std::uint8_t>(b);
        ip.data[2] = static_cast<std::uint8_t>(c);
        ip.data[3] = static_cast<std::uint8_t>(d);

        return is;
    }
};

TEST(IPV4, COMPARISON) {
    IPv4 ip7(192, 168, 1, 1);
    IPv4 ip8(192, 168, 1, 2);

    ASSERT_TRUE(ip7 == ip7);
    ASSERT_TRUE(ip7 != ip8);
    ASSERT_TRUE(ip7 < ip8);
    ASSERT_TRUE(ip7 <= ip7);
    ASSERT_TRUE(ip7 <= ip8);
}

TEST(IPV4, ICREMENT) {
    IPv4 ip1(192, 168, 1, 1);
    ++ip1;
    ASSERT_EQ(ip1, IPv4(192, 168, 1, 2));

    IPv4 ip2(192, 168, 1, 1);
    IPv4 ip3 = ip2++;
    ASSERT_EQ(ip2, IPv4(192, 168, 1, 2));
    ASSERT_EQ(ip3, IPv4(192, 168, 1, 1));

    IPv4 ip4(192, 168, 1, 255);
    ++ip4;
    ASSERT_EQ(ip4, IPv4(192, 168, 2, 0));

    IPv4 ip5(192, 168, 255, 255);
    ++ip5;
    ASSERT_EQ(ip5, IPv4(192, 169, 0, 0));

    IPv4 ip6(255, 255, 255, 255);
    ++ip6;
    ASSERT_EQ(ip6, IPv4(0, 0, 0, 0));
}

TEST(IPV4, DECREMENT) {
    IPv4 ip1(192, 168, 1, 2);
    --ip1;
    ASSERT_EQ(ip1, IPv4(192, 168, 1, 1));

    IPv4 ip2(192, 168, 1, 2);
    IPv4 ip3 = ip2--;
    ASSERT_EQ(ip2, IPv4(192, 168, 1, 1));
    ASSERT_EQ(ip3, IPv4(192, 168, 1, 2));

    IPv4 ip4(192, 168, 2, 0);
    --ip4;
    ASSERT_EQ(ip4, IPv4(192, 168, 1, 255));

    IPv4 ip5(192, 169, 0, 0);
    --ip5;
    ASSERT_EQ(ip5, IPv4(192, 168, 255, 255));

    IPv4 ip6(0, 0, 0, 0);
    --ip6;
    ASSERT_EQ(ip6, IPv4(255, 255, 255, 255));
}

TEST(IPV4, INPUT_OUTPUT) {
    IPv4 ip1(192, 168, 1, 1);
    std::ostringstream oss;
    oss << ip1;
    ASSERT_EQ(oss.str(), "192.168.1.1");

    IPv4 ip2;
    std::istringstream iss("10.0.0.1");
    iss >> ip2;
    ASSERT_EQ(ip2, IPv4(10, 0, 0, 1));
}