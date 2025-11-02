//
// Created by ivankhripunov on 02.11.2025.
//

#include <numbers>
#include <cmath>
#include <vector>
#include <gtest/gtest.h>

class Shape {
public:
    virtual ~Shape() = default;

    [[nodiscard]] virtual double perimeter() const = 0;

    [[nodiscard]] virtual double area() const = 0;
};

class Triangle final : public Shape {
private:
    double side_a;
    double side_b;
    double side_c;

public:
    Triangle(double a, double b, double c) : side_a(a), side_b(b), side_c(c) {}

    [[nodiscard]] double perimeter() const override final {
        return side_a + side_b + side_c;
    }

    [[nodiscard]] double area() const override final {
        const double p = perimeter() / 2.0;
        return std::sqrt(p * (p - side_a) * (p - side_b) * (p - side_c));
    }

    [[nodiscard]] double get_side_a() const { return side_a; }

    [[nodiscard]] double get_side_b() const { return side_b; }

    [[nodiscard]] double get_side_c() const { return side_c; }
};

class Square final : public Shape {
private:
    double side;

public:
    Square(double s) : side(s) {}

    [[nodiscard]] double perimeter() const override final {
        return 4.0 * side;
    }

    [[nodiscard]] double area() const override final {
        return side * side;
    }

    [[nodiscard]] double get_side() const { return side; }
};

class Circle final : public Shape {
private:
    double radius;

public:
    Circle(double r) : radius(r) {}

    [[nodiscard]] double perimeter() const override final {
        return 2.0 * std::numbers::pi * radius;
    }

    [[nodiscard]] double area() const override final {
        return std::numbers::pi * radius * radius;
    }

    [[nodiscard]] double get_radius() const { return radius; }
};

TEST(SHAPE, VECTOR) {
    std::vector<Shape *> shapes;
    shapes.reserve(3);

    Triangle triangle(3, 4, 5);
    Square square(2);
    Circle circle(1);

    shapes.push_back(&triangle);
    shapes.push_back(&square);
    shapes.push_back(&circle);

    ASSERT_DOUBLE_EQ(shapes[0]->area(), 6);
    ASSERT_DOUBLE_EQ(shapes[1]->area(), 4);
    ASSERT_DOUBLE_EQ(shapes[2]->area(), std::numbers::pi);

    ASSERT_DOUBLE_EQ(shapes[0]->perimeter(), 12);
    ASSERT_DOUBLE_EQ(shapes[1]->perimeter(), 8);
    ASSERT_DOUBLE_EQ(shapes[2]->perimeter(), 2 * std::numbers::pi);
}
