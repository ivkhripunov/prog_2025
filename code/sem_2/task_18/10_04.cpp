#include <gtest/gtest.h>
#include <cstddef>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/operation.hpp>

using Matrix = boost::numeric::ublas::matrix<std::size_t>;

[[nodiscard]] Matrix power(Matrix m, std::size_t n) {
    Matrix result(2, 2, 0);
    result(0, 0) = 1;
    result(1, 1) = 1;

    while (n > 0) {
        if (n & 1) result = Matrix(boost::numeric::ublas::prod(result, m));

        m = Matrix(boost::numeric::ublas::prod(m, m));

        n >>= 1;
    }

    return result;
}

[[nodiscard]] std::size_t fibonacci_matrix(const std::size_t n) {
    if (n == 0) return 0;

    Matrix base(2, 2);

    base(0, 0) = 1;
    base(0, 1) = 1;
    base(1, 0) = 1;
    base(1, 1) = 0;

    return power(base, n)(0, 1);
}

TEST(fibonacci, matrix) {
    ASSERT_EQ(fibonacci_matrix( 0), 0);
    ASSERT_EQ(fibonacci_matrix( 1), 1);
    ASSERT_EQ(fibonacci_matrix( 2), 1);
    ASSERT_EQ(fibonacci_matrix( 3), 2);
    ASSERT_EQ(fibonacci_matrix( 5), 5);
    ASSERT_EQ(fibonacci_matrix(92), 7540113804746346429);
}

// Алгоритмическая сложность:
//
// [1 1]^n = [F(n+1) F(n)]
// [1 0]     [F(n)   F(n-1)]
//
// Возведение матрицы в степень может осуществлено с
// логарифмической сложностью O(log n).
// Это и определяет сложность реализованного в этой задаче подхода.
// Итеративный расчет имеет сложность O(n), рекурсивный -- O(2^n).
// Таким образом, для больших значений n из рассмотренных алгоритмов матричный наименее ресурсоемкий.
