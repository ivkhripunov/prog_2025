//
// Created by ivankhripunov on 01.11.2025.
//

#include <gtest/gtest.h>
#include <numbers>
#include <cmath>

class Triangle {
private:
    double side_a;
    double side_b;
    double side_c;

public:
    Triangle(const double a, const double b, const double c) : side_a(a), side_b(b), side_c(c) {}

    [[nodiscard]] double perimeter() const {
        return side_a + side_b + side_c;
    }

    [[nodiscard]] double area() const {
        const double p = perimeter() / 2.0;
        return std::sqrt(p * (p - side_a) * (p - side_b) * (p - side_c));
    }

    [[nodiscard]] double get_side_a() const { return side_a; }

    [[nodiscard]] double get_side_b() const { return side_b; }

    [[nodiscard]] double get_side_c() const { return side_c; }
};

class Square {
private:
    double side;

public:
    Square(const double s) : side(s) {}

    [[nodiscard]] double perimeter() const {
        return 4.0 * side;
    }

    [[nodiscard]] double area() const {
        return side * side;
    }

    [[nodiscard]] double get_side() const { return side; }
};

class Circle {
private:
    double radius;

public:
    Circle(const double r) : radius(r) {}

    [[nodiscard]] double perimeter() const {
        return 2.0 * std::numbers::pi * radius;
    }

    [[nodiscard]] double area() const {
        return std::numbers::pi * radius * radius;
    }

    [[nodiscard]] double get_radius() const { return radius; }
};

TEST(TASK_03_03, TRIANGLE) {
    const Triangle triangle = {3, 4, 5};

    ASSERT_DOUBLE_EQ(triangle.perimeter(), 12.0);

    ASSERT_DOUBLE_EQ(triangle.area(), 6.0);

    ASSERT_DOUBLE_EQ(triangle.get_side_a(), 3.0);
    ASSERT_DOUBLE_EQ(triangle.get_side_b(), 4.0);
    ASSERT_DOUBLE_EQ(triangle.get_side_c(), 5.0);
}

TEST(TASK_03_03, SQUARE) {
    const Square square{5};

    ASSERT_DOUBLE_EQ(square.perimeter(), 20.0);

    ASSERT_DOUBLE_EQ(square.area(), 25.0);

    ASSERT_DOUBLE_EQ(square.get_side(), 5.0);
}

TEST(TASK_03_04, CIRCLE) {
    constexpr double pi = std::numbers::pi;

    const Circle square{5};

    ASSERT_DOUBLE_EQ(square.perimeter(), 2 * pi * 5);

    ASSERT_DOUBLE_EQ(square.area(), pi * 5 * 5);

    ASSERT_DOUBLE_EQ(square.get_radius(), 5);
}