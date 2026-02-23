////////////////////////////////////////////////////////////////////////////////////

// support : www.cs.usfca.edu/~galles/visualization/ComparisonSort.html

////////////////////////////////////////////////////////////////////////////////////

#include <algorithm>
#include <numeric>
#include <utility>
#include <vector>

#include <benchmark/benchmark.h>
#include <gtest/gtest.h>

////////////////////////////////////////////////////////////////////////////////////

template<typename T>
[[nodiscard]] T median_of_three(const T &a, const T &b, const T &c) {
    return std::clamp(a, std::min(b, c), std::max(b, c));
}

////////////////////////////////////////////////////////////////////////////////////

template<typename T>
void order(std::vector<T> &vector, const std::size_t left, const std::size_t right) {
    for (auto i = left + 1; i < right; ++i) {
        for (auto j = i; j > left; --j) {
            if (vector[j - 1] > vector[j]) {
                std::swap(vector[j], vector[j - 1]);
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////

template<typename T>
void split(std::vector<T> &vector,
           const std::size_t left,
           const std::size_t right,
           const std::size_t threshold) {
    if (right - left > threshold) {
        const auto middle = left + (right - left) / 2;
        const T pivot = median_of_three(vector.front(), vector[middle], vector.back());

        std::size_t i = left;
        std::size_t j = right - 1;

        while (i <= j) {
            while (vector[i] < pivot) ++i;
            while (vector[j] > pivot) --j;

            if (i <= j) {
                if (i != j) {
                    std::swap(vector[i], vector[j]);
                }
                ++i;
                --j;
            }
        }

        if (left < j) split(vector, left, j + 1, threshold);
        if (i < right) split(vector, i, right, threshold);
    } else {
        order(vector, left, right);
    }
}

////////////////////////////////////////////////////////////////////////////////////

template<typename T>
void sort(std::vector<T> &vector, const std::size_t threshold = 16) {
    if (vector.size() > 1) {
        split(vector, 0, vector.size(), threshold);
    }
}

static std::vector<double> make_reverse_sorted(const std::size_t n) {
    std::vector<double> v(n);
    std::iota(v.rbegin(), v.rend(), 1.0);
    return v;
}

TEST(TASK_07_04, BENCHMARK) {
    constexpr std::size_t size = 10000;

    int argc = 0;
    char *argv = nullptr;
    benchmark::Initialize(&argc, &argv);

    const auto benchmark = [](benchmark::State &state) {
        const auto threshold = static_cast<std::size_t>(state.range(0));
        const auto source = make_reverse_sorted(size);

        for (auto _: state) {
            auto vector = source;
            sort(vector, threshold);
            benchmark::DoNotOptimize(vector.data());
            benchmark::ClobberMemory();
        }
    };

    benchmark::RegisterBenchmark("quick_sort_benchmark", benchmark)
            ->RangeMultiplier(2)->Range(2, 256)->Unit(benchmark::kMicrosecond);

    benchmark::RunSpecifiedBenchmarks();
    benchmark::Shutdown();

    SUCCEED();
}
