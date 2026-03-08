/////////////////////////////////////////////////////////////////////////////////

// chapter : Number Processing

/////////////////////////////////////////////////////////////////////////////////

// section : Chrono Management

/////////////////////////////////////////////////////////////////////////////////

// content : Timing

/////////////////////////////////////////////////////////////////////////////////

#include <chrono>
#include <cmath>
#include <cstddef>
#include <numeric>
#include <print>
#include <stdexcept>
#include <string>
#include <vector>
#include <gtest/gtest.h>

/////////////////////////////////////////////////////////////////////////////////

template<typename D = std::chrono::duration<double> >
class Timer {
public :
    explicit Timer(std::string const &scope)
        : m_scope(scope), m_running(false), m_begin() {
    }

    //  -----------------------------------------------------------------------------
    ~Timer() {
        if (!m_durations.empty()) {
            std::print("{} : average = {:.6f}\n", m_scope, average());
        }
    }

    //  -----------------------------------------------------------------------------
    void start() {
        if (m_running) {
            throw std::runtime_error("already running");
        }
        m_running = true;
        m_begin = clock_t::now();
    }

    //  -----------------------------------------------------------------------------
    void stop() {
        if (!m_running) {
            throw std::runtime_error("not running");
        }
        m_durations.push_back(
            std::chrono::duration_cast<D>(clock_t::now() - m_begin));

        m_running = false;
    }

    //  -----------------------------------------------------------------------------
    [[nodiscard]] double average() const {
        if (m_durations.empty()) {
            throw std::runtime_error("no measurements");
        }
        auto total = std::accumulate(
            m_durations.cbegin(),
            m_durations.cend(),
            D{},
            [](D acc, D const &d) { return acc + d; });

        return total.count() / static_cast<double>(m_durations.size());
    }

    //  -----------------------------------------------------------------------------
    [[nodiscard]] std::size_t size() const noexcept { return m_durations.size(); }

private :
    using clock_t = std::chrono::steady_clock;
    //  -----------------------------------------------------------------------------
    std::string m_scope;
    bool m_running;
    clock_t::time_point m_begin;
    std::vector<D> m_durations;
};

/////////////////////////////////////////////////////////////////////////////////
TEST(task_08_05, timer) {
    constexpr std::size_t N = 5;
    constexpr std::size_t size = 1'000'000;

    const auto calculate = []() -> void {
        double sum = 0;
        for (auto i = 0uz; i < size; ++i) {
            sum += std::pow(std::sin(i), 2) + std::pow(std::cos(i), 2);
        }
    };

    Timer timer("main : timer");

    for (std::size_t i = 0; i < N; ++i) {
        ASSERT_EQ(timer.size(), i);
        timer.start();
        ASSERT_THROW(timer.start(), std::runtime_error);
        calculate();
        timer.stop();
        ASSERT_THROW(timer.stop(), std::runtime_error);
    }

    std::print("Completed {} measurements\n", timer.size());
    std::print("Average time          : {:.6f} s\n", timer.average());
}

/////////////////////////////////////////////////////////////////////////////////
