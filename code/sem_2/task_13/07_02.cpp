//
// Created by ivankhripunov on 02.11.2025.
//

///////////////////////////////////////////////////////////////////////////////////////////////

#include <cmath>
#include <numeric>
#include <ostream>
#include <vector>
#include <variant>
#include <exception>
#include <iostream>
#include <optional>

///////////////////////////////////////////////////////////////////////////////////////////////

template<typename Derived>
class addable {
    friend Derived operator+(Derived lhs, const Derived &rhs) { return lhs += rhs; }
};

template<typename Derived>
class subtractable {
    friend Derived operator-(Derived lhs, const Derived &rhs) { return lhs -= rhs; }
};

template<typename Derived>
class multipliable {
    friend Derived operator*(Derived lhs, const Derived &rhs) { return lhs *= rhs; }
};

template<typename Derived>
class dividable {
    friend Derived operator/(Derived lhs, const Derived &rhs) { return lhs /= rhs; }
};

template<typename Derived>
class incrementable {
    friend Derived &operator++(Derived &obj) { return obj += 1; }

    friend Derived operator++(Derived &obj, int) {
        Derived tmp(obj);
        ++obj;
        return tmp;
    }
};

template<typename Derived>
class decrementable {
    friend Derived &operator--(Derived &obj) { return obj -= 1; }

    friend Derived operator--(Derived &obj, int) {
        Derived tmp(obj);
        --obj;
        return tmp;
    }
};

///////////////////////////////////////////////////////////////////////////////////////////////

class Exception : public std::exception {
    const char *m_message;

public:
    explicit Exception(const char *message) : m_message(message) {
    }

    const char *what() const noexcept override {
        return m_message;
    }
};

///////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
class Rational : public addable<Rational<T> >,
                 public subtractable<Rational<T> >,
                 public multipliable<Rational<T> >,
                 public dividable<Rational<T> >,
                 public incrementable<Rational<T> >,
                 public decrementable<Rational<T> > {
public :
    Rational(T num = 0, T den = 1) : m_num(num), m_den(den) {
        if (den == 0) throw Exception("Zero denominator");
        reduce();
    }

    //  -------------------------------------------------------------------------------------------

    explicit operator double() const {
        return 1.0 * m_num / m_den;
    }

    //  -------------------------------------------------------------------------------------------

    auto &operator+=(Rational const &other) {
        auto lcm = std::lcm(m_den, other.m_den);

        m_num = m_num * (lcm / m_den) + other.m_num * (lcm / other.m_den);

        m_den = lcm;

        reduce();

        return *this;
    }

    //  -------------------------------------------------------------------------------------------

    auto &operator-=(Rational const &other) {
        return *this += Rational(other.m_num * -1, other.m_den);
    }

    //  -------------------------------------------------------------------------------------------

    auto &operator*=(Rational const &other) {
        m_num *= other.m_num;

        m_den *= other.m_den;

        reduce();

        return *this;
    }

    //  -------------------------------------------------------------------------------------------

    auto &operator/=(Rational const &other) {
        return *this *= Rational(other.m_den, other.m_num);
    }

    //  -------------------------------------------------------------------------------------------


    friend bool operator==(Rational const &lhs, Rational const &rhs) {
        return lhs.m_num * rhs.m_den == rhs.m_num * lhs.m_den;
    }

    //  -------------------------------------------------------------------------------------------

    friend auto &operator>>(std::istream &stream, Rational &rational) {
        return (stream >> rational.m_num).ignore() >> rational.m_den;
    }

    //  -------------------------------------------------------------------------------------------

    friend auto &operator<<(std::ostream &stream, Rational const &rational) {
        return stream << rational.m_num << '/' << rational.m_den;
    }

private :
    void reduce() {
        if (m_den < 0) {
            m_num = -m_num;

            m_den = -m_den;
        }

        auto gcd = std::gcd(m_num, m_den);

        m_num /= gcd;

        m_den /= gcd;
    }

    //  -------------------------------------------------------------------------------------------

    T m_num = 0, m_den = 1;
};


///////////////////////////////////////////////////////////////////////////////////////////////

auto equal(double x, double y, double epsilon = 1e-6) {
    return std::abs(x - y) < epsilon;
}

///////////////////////////////////////////////////////////////////////////////////////////////

int main() {
    try {
        Rational r(1, 0);
    } catch (const Exception &e) {
        std::cerr << "Caught Exception: " << e.what() << std::endl;
    }

    try {
        std::vector<int> v;
        v.reserve(std::numeric_limits<size_t>::max());
    } catch (const std::length_error &e) {
        std::cerr << "Caught length_error: " << e.what() << std::endl;
    }

    try {
        size_t n = 1000000000000;
        int *p = new int[n];
        delete[] p;
    } catch (const std::bad_alloc &e) {
        std::cerr << "Caught bad_alloc: " << e.what() << std::endl;
    }

    try {
        std::variant<int, double> v = 328;
        std::get<double>(v);
    } catch (const std::bad_variant_access &e) {
        std::cerr << "Caught bad_variant_access: " << e.what() << std::endl;
    }

    try {
        std::optional<int> opt;
        opt.value();
    } catch (const std::bad_optional_access &e) {
        std::cerr << "Caught bad_optional_access: " << e.what() << std::endl;
    }

    try {
        std::vector vec = {1, 2, 3};
        vec.at(10);
    } catch (const std::out_of_range &e) {
        std::cerr << "Caught out_of_range: " << e.what() << std::endl;
    }

    return 0;

    /// Пояснение:
    /// std::bad_alloc возникает при попытке выделить память, когда свободной памяти недостаточно для удовлетворения запроса;
    /// std::bad_variant_access возникает при попытке получить значение из std::variant не того типа, который в данный момент хранится в варианте;
    /// std::bad_optional_access возникает при попытке обратиться к значению пустого std::optional;
    /// std::length_error возникает при попытке превысить максимально допустимый размер контейнераю;
    /// std::out_of_range возникает при попытке доступа к элементу контейнера через at() с индексом, выходящим за допустимые границы.
}
