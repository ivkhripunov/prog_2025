#include <gtest/gtest.h>
#include <type_traits>

class Wrapper {
public :
    using function_t = Wrapper(*)();

    explicit Wrapper(const function_t function) : m_function(function) {
    }

    operator function_t() const { return m_function; }

private :
    function_t m_function;
};

Wrapper test() {
    return Wrapper(test);
}

TEST(Wrapper, type) {
    static_assert(std::is_same_v<decltype(&test), Wrapper(*)()>);
}

TEST(Wrapper, call) {
    const Wrapper function = test();

    const Wrapper result = (*function)();

    ASSERT_EQ(static_cast<Wrapper::function_t>(result), test);
}

TEST(Wrapper, chain) {
    const Wrapper a = test();
    const Wrapper b = (*a)();
    const Wrapper c = (*b)();

    ASSERT_EQ(static_cast<Wrapper::function_t>(a), test);
    ASSERT_EQ(static_cast<Wrapper::function_t>(b), test);
    ASSERT_EQ(static_cast<Wrapper::function_t>(c), test);
}

TEST(Wrapper, conversion) {
    const Wrapper function = test();

    const Wrapper::function_t ptr = function;

    ASSERT_EQ(ptr, test);
}
