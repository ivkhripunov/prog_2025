//
// Created by ivan on 11.05.2026.
//

#include <boost/graph/adjacency_matrix.hpp>
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>
#include <limits>

using Graph = boost::adjacency_matrix<boost::undirectedS, boost::no_property,
    boost::property<boost::edge_weight_t, int> >;

int main() {
    constexpr std::size_t N = 10;

    std::random_device rd;
    std::default_random_engine engine(rd());
    std::uniform_int_distribution<int> dist(1, 10);

    Graph graph(N);
    const auto weight = boost::get(boost::edge_weight, graph);

    for (std::size_t i = 0; i < N; ++i) {
        for (std::size_t j = i + 1; j < N; ++j) {
            const auto [edge, added] = boost::add_edge(i, j, graph);
            boost::put(weight, edge, dist(engine));
        }
    }

    const auto get_weight = [&](const std::size_t u, const std::size_t v) -> int {
        const auto [edge, exists] = boost::edge(u, v, graph);
        return boost::get(weight, edge);
    };

    const auto path_cost = [&](const std::vector<std::size_t> &path) -> int {
        int cost = 0;
        for (std::size_t i = 0; i < path.size() - 1; ++i) {
            cost += get_weight(path[i], path[i + 1]);
        }
        return cost + get_weight(path.back(), path.front());
    };

    std::vector<std::size_t> vertices(N);
    std::iota(vertices.begin(), vertices.end(), 0);

    int best_cost = std::numeric_limits<int>::max();
    std::vector<std::size_t> best_path;

    do {
        if (const int cost = path_cost(vertices); cost < best_cost) {
            best_cost = cost;
            best_path = vertices;
        }
    } while (std::next_permutation(vertices.begin(), vertices.end()));

    std::cout << "Матрица инцидентности:\n";
    for (std::size_t i = 0; i < N; ++i) {
        for (std::size_t j = 0; j < N; ++j) {
            std::cout << std::setw(4) << get_weight(i, j);
        }
        std::cout << "\n";
    }

    std::cout << "Оптимальный обход вершин: ";
    for (const auto v: best_path) {
        std::cout << v << " -> ";
    }
    std::cout << best_path[0] << std::endl;
    std::cout << "Суммарная стоимость пути: " << best_cost << std::endl;
}
