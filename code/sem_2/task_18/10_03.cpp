#include <boost/multi_array.hpp>

#include <iostream>

using Grid = boost::multi_array<int, 2>;

[[nodiscard]] int neighbors(const Grid &g, const int r, const int c) {
    const auto s = static_cast<int>(g.shape()[0]);

    int n = 0;
    for (auto dr = -1; dr <= 1; ++dr)
        for (auto dc = -1; dc <= 1; ++dc)
            if ((dr || dc) && r + dr >= 0 && c + dc >= 0 && r + dr < s && c + dc < s)
                n += g[r + dr][c + dc];

    return n;
}

[[nodiscard]] Grid next(const Grid &g) {
    const std::size_t size = g.shape()[0];
    Grid next(boost::extents[size][size]);

    for (auto i = 0uz; i < size; ++i)
        for (auto j = 0uz; j < size; ++j) {
            auto n = neighbors(g, i, j);
            next[i][j] = g[i][j] ? (n == 2 || n == 3) : (n == 3);
        }

    return next;
}

void print(const Grid &g, const std::size_t generation) {
    const std::size_t size = g.shape()[0];

    std::cout << '\n' << "generation " << generation << '\n';

    for (auto i = 0uz; i < size; ++i) {
        for (auto j = 0uz; j < size; ++j)
            std::cout << (g[i][j] ? '#' : '-');
        std::cout << '\n';
    }
}

int main() {
    auto size = 10uz;

    Grid g(boost::extents[size][size]);

    // Фигура глайдер
    g[0][1] = g[1][2] = g[2][0] = g[2][1] = g[2][2] = 1;

    for (auto i = 0uz; i <= 20; ++i, g = next(g))
        print(g, i);
}
