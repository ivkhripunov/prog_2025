#include <array>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <random>
#include <set>
#include <string>
#include <unordered_set>
#include <vector>

////////////////////////////////////////////////////////////////////////////////

[[nodiscard]] unsigned int RS_hash(const std::string &str) {
    constexpr unsigned int b = 378551;

    unsigned int a = 63689;
    unsigned int hash = 0;

    for (const auto c: str) {
        hash = hash * a + static_cast<unsigned char>(c);
        a *= b;
    }

    return hash;
}

////////////////////////////////////////////////////////////////////////////////

[[nodiscard]] unsigned int JS_hash(const std::string &str) {
    unsigned int hash = 1315423911;

    for (const auto c: str)
        hash ^= (hash << 5) + static_cast<unsigned char>(c) + (hash >> 2);

    return hash;
}

////////////////////////////////////////////////////////////////////////////////

[[nodiscard]] unsigned int PJW_hash(const std::string &str) {
    constexpr unsigned int bits = sizeof(unsigned int) * 8;
    constexpr unsigned int q = bits * 3 / 4;
    constexpr unsigned int eighth = bits / 8;
    constexpr unsigned int high = 0xFFFFFFFFu << (bits - eighth);

    unsigned int hash = 0;
    unsigned int test = 0;

    for (const auto c: str) {
        hash = (hash << eighth) + static_cast<unsigned char>(c);

        if ((test = hash & high) != 0)
            hash = (hash ^ (test >> q)) & ~high;
    }

    return hash;
}

////////////////////////////////////////////////////////////////////////////////

[[nodiscard]] unsigned int ELF_hash(const std::string &str) {
    unsigned int hash = 0;
    unsigned int x = 0;

    for (const auto c: str) {
        hash = (hash << 4) + static_cast<unsigned char>(c);

        if ((x = hash & 0xF0000000u) != 0)
            hash ^= (x >> 24);

        hash &= ~x;
    }

    return hash;
}

////////////////////////////////////////////////////////////////////////////////

[[nodiscard]] unsigned int BKDR_hash(const std::string &str) {
    constexpr unsigned int seed = 131;

    unsigned int hash = 0;

    for (const auto c: str)
        hash = hash * seed + static_cast<unsigned char>(c);

    return hash;
}

////////////////////////////////////////////////////////////////////////////////

[[nodiscard]] unsigned int SDBM_hash(const std::string &str) {
    unsigned int hash = 0;

    for (const auto c: str)
        hash = static_cast<unsigned char>(c) + (hash << 6) + (hash << 16) - hash;

    return hash;
}

////////////////////////////////////////////////////////////////////////////////

[[nodiscard]] unsigned int DJB_hash(const std::string &str) {
    unsigned int hash = 5381;

    for (const auto c: str)
        hash = ((hash << 5) + hash) + static_cast<unsigned char>(c);

    return hash;
}

////////////////////////////////////////////////////////////////////////////////

[[nodiscard]] unsigned int DEK_hash(const std::string &str) {
    auto hash = static_cast<unsigned int>(str.size());

    for (const auto c: str)
        hash = ((hash << 5) ^ (hash >> 27)) ^ static_cast<unsigned char>(c);

    return hash;
}

////////////////////////////////////////////////////////////////////////////////

[[nodiscard]] unsigned int AP_hash(const std::string &str) {
    unsigned int hash = 0xAAAAAAAAu;

    for (auto i = 0u; i < str.size(); ++i) {
        const auto c = static_cast<unsigned char>(str[i]);

        hash ^= (i & 1) == 0
                    ? (hash << 7) ^ c * (hash >> 3)
                    : ~((hash << 11) + (c ^ (hash >> 5)));
    }

    return hash;
}

////////////////////////////////////////////////////////////////////////////////

[[nodiscard]] std::vector<std::string> make_strings(const std::size_t count,
                                                    const std::size_t length) {
    std::set<std::string> pool;
    std::string s(length, '_');

    std::uniform_int_distribution dist(97, 122);
    std::default_random_engine engine;

    while (pool.size() < count) {
        for (auto &ch: s)
            ch = dist(engine);

        pool.insert(s);
    }

    return {pool.begin(), pool.end()};
}

////////////////////////////////////////////////////////////////////////////////

int main() {
    constexpr std::size_t total = 1u << 20;
    constexpr std::size_t str_len = 50;
    constexpr std::size_t step = 1u << 10;

    // -------------------------------------------------------------------------

    const auto strings = make_strings(total, str_len);

    // -------------------------------------------------------------------------

    using fn_t = unsigned int (*)(const std::string &);

    constexpr std::size_t num_fns = 9;

    constexpr std::array<const char *, num_fns> names =
    {
        "RS", "JS", "PJW", "ELF", "BKDR", "SDBM", "DJB", "DEK", "AP"
    };

    constexpr std::array<fn_t, num_fns> fns =
    {
        RS_hash, JS_hash, PJW_hash, ELF_hash,
        BKDR_hash, SDBM_hash, DJB_hash, DEK_hash, AP_hash
    };

    // -------------------------------------------------------------------------

    std::array<std::unordered_set<unsigned int>, num_fns> seen;
    std::array<std::size_t, num_fns> collisions{};

    for (auto &s: seen)
        s.reserve(total);

    // -------------------------------------------------------------------------

    const std::string csv_path = "collisions.csv";

    std::ofstream csv(csv_path);

    // -------------------------------------------------------------------------

    csv << "n";
    for (const auto *name: names)
        csv << ',' << name;
    csv << '\n';

    // -------------------------------------------------------------------------

    for (std::size_t i = 0; i < strings.size(); ++i) {
        for (std::size_t j = 0; j < num_fns; ++j) {
            const unsigned int h = fns[j](strings[i]);

            if (!seen[j].insert(h).second)
                ++collisions[j];
        }

        if (i % step == 0) {
            csv << i;

            for (const auto c: collisions)
                csv << ',' << c;

            csv << '\n';
        }
    }

    csv << strings.size();
    for (const auto c: collisions)
        csv << ',' << c;
    csv << '\n';

    return 0;
}

////////////////////////////////////////////////////////////////////////////////

// Все кривые растут нелинейно.
//
// При N << M кривая почти горизонтальна, затем резко уходит вверх.
// По теории вероятностей (случайное размещение N шаров в M ячейках):
//
// 1. Вероятность того, что конкретная ячейка не занята после N вставок:
//
//     P = (1 − 1/M) ^ N
//
// 2. Ожидаемое число занятых ячеек:
//
//     M · (1 − (1 − 1/M) ^ N)
//
// 3. Ожидаемое число коллизий:
//
//     C(N) = N − M · (1 − (1 − 1/M) ^ N)
//
// При N << M формула упрощается до C(N) = N² / (2M).
//
// До N = 50 000 большинтво функций дают ноль коллизий.
// После кривые расходятся.
//
// Первая коллизия
// -----------------------------------
//   PJW, ELF :  23 500
//   BKDR     :  33 800
//   SDBM     :  49 200
//   RS, DEK  :  57 300
//   AP       :  95 200
//   JS       :  99 300
//   DJB      :  165 900
//
// Результаты при N = 1 048 576 (по теоретической оценке должно быть 128)
// -------------------------------------------
//   SDBM :  120
//   JS   :  130
//   BKDR :  135
//   DJB  :  138
//   AP   :  138
//   RS   :  150
//   DEK  :  208
//   PJW  : 2750
//   ELF  : 2750
//
// Лучшие функции: SDBM, JS, BKDR, DJB, AP
// Все пять дают коллизии окрестности теортического минимума,
// это признак равномерного покрытия всего пространства.
//
// DJB выделяется позднейшей первой коллизией (N = 166 000): мультипликатор 33
// и аддитивная схема hash = hash * 33 + c дают хорошее рассеивание
// на случайных строках при умеренном N.
//
// SDBM использует мультипликатор 65 599 = 2^16 + 2^6 − 1 — простое число
// с равномерным битовым паттерном; реализуется сдвигами без умножения:
//
//   hash = c + (hash << 6) + (hash << 16) − hash
//
// Это обеспечивает хорошее перемешивание всех 32 бит при малых и средних N.
//
// DEK показывает худший результат среди группы лучших.
// hash = (hash << 5) ^ (hash >> 27) ^ c.
// Начальное значение seed = len(str) = 50 для всех строк — константа.
// Это сужает начальное разнообразие состояний и приводит к незначительной
// кластеризации значений, заметной лишь при больших N.
//
// Худшие функции: PJW и ELF
// PJW и ELF дают схожие резльтаты на всём диапазоне N.
// Оба алгоритма усекают выходной диапазон, удаляя старшие биты через маску.
// Эффективное пространство хешей: 2^28 = 268 · 10^6 вместо 2^32 = 4.3 · 10^9.
// По парадоксу дней рождения это увеличивает ожидаемое число коллизий в
// 2^32 / 2^28 = 16 раз, что близко к наблюдаемому значению (примерно 22 раз).
//

////////////////////////////////////////////////////////////////////////////////


/*
 * 
 * Score is 5/10
 * You can resubmit and change to 9/10, if you want
 * 
 * Your process is correct, you export the data file and plot it correctly
 * 
 * But
 * 
 * Your result does not align to what is expected, we expect 1-30 collisions not 40k
 * It is most probably beacuse, your string generator is not correct
 * You can take the string generator from the Teacher's template: https://github.com/i-s-m-mipt/Education/blob/master/projects/examples/source/10.42.cpp
 * 
 * If you want to see a sample of results, you can look https://github.com/kafishabbir/hash-functions
 */
