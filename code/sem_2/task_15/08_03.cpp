//
// Created by ivan on 08.03.2026.
//

#include <climits>
#include <iostream>
#include <gtest/gtest.h>

// В реализациях функций ниже проверка на положительность входных чисел не выполнялась,
// так как входные параметры по условию задачи положительные.

// Побитовый сдвиг эквивалентен делению пополам.
// Делаем сдвиги, набираем степень, пока значение больше 1.
[[nodiscard]] int log_int(const int n) {
    auto val = static_cast<unsigned int>(n);

    int result = 0;
    while (val > 1) {
        val >>= 1;
        result++;
    }
    return result;
}


[[nodiscard]] int log_floor(const float x) {
    union {
        float f;
        unsigned int u;
    } conv;

    conv.f = x;
    const unsigned int bits = conv.u;

    // Для извлечения бит экмпоненты и мантиссы используем маски.
    const unsigned int exponent = (bits >> 23) & 0xFFU;
    const unsigned int significand = bits & 0x7FFFFFU;

    // Обработка случая INF и NAN
    if (exponent == 0xFFU) {
        throw std::runtime_error("bad input");
    }

    // Обработка нормализованных чисел
    if (exponent != 0U) {
        return static_cast<int>(exponent) - 127;
    }

    // Обработка денормализованных чисел.
    // floor(log2(x)) = (p - 23) + (-126) = p - 149,
    // где p -- позиция старшего нулевого бита мантиссы.
    return log_int(static_cast<int>(significand)) - 149;
}

TEST(task_08_03, pow_2) {
    EXPECT_EQ(log_int(1), 0);
    EXPECT_EQ(log_int(1024), 10);
    EXPECT_EQ(log_floor(1.0f), 0);
    EXPECT_EQ(log_floor(0.25f), -2);
}

TEST(task_08_03, round_down) {
    EXPECT_EQ(log_int(7), 2);
    EXPECT_EQ(log_floor(0.75f), -1);
}

TEST(task_08_03, denormalized) {
    unsigned int bits_max_denorm = 0x007FFFFFU;
    float max_denorm;
    memcpy(&max_denorm, &bits_max_denorm, sizeof(max_denorm));
    EXPECT_EQ(log_floor(max_denorm), -127);

    unsigned int bits_min_denorm = 0x00000001U;
    float min_denorm;
    memcpy(&min_denorm, &bits_min_denorm, sizeof(min_denorm));
    EXPECT_EQ(log_floor(min_denorm), -149);
}

TEST(task_08_03, bad_input) {
    unsigned int bits_inf = 0x7F800000U;
    float inf_val;
    memcpy(&inf_val, &bits_inf, sizeof(inf_val));
    ASSERT_ANY_THROW([[maybe_unused]] auto x = log_floor(inf_val));
}
