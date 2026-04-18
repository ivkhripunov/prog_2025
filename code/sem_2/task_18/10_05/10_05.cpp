#include <iostream>
#include <ranges>
#include <string>
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

[[nodiscard]] static std::size_t collisions(const std::vector<std::string> &strings,
                                            unsigned int (*fn)(const std::string &),
                                            const unsigned int table_size) {
    std::vector occupied(table_size, false);

    std::size_t count = 0;

    for (const auto &s: strings) {
        const auto bucket = fn(s) % table_size;

        if (occupied[bucket]) ++count;
        else occupied[bucket] = true;
    }

    return count;
}

////////////////////////////////////////////////////////////////////////////////

int main() {
    constexpr unsigned int table_size = 100003;

    const std::vector<std::size_t> ns =
    {
        1000, 2000, 5000, 10000, 20000, 50000, 100000
    };
	// More smooth graph is required for example record collision data every 1000 step, 100 points on the x axis
	
    using fn_t = unsigned int (*)(const std::string &); // use std::function

    const std::vector<std::pair<const char *, fn_t> > fns =
    {
        {"RS", RS_hash},
        {"JS", JS_hash},
        {"PJW", PJW_hash},
        {"ELF", ELF_hash},
        {"BKDR", BKDR_hash},
        {"SDBM", SDBM_hash},
        {"DJB", DJB_hash},
        {"DEK", DEK_hash},
        {"AP", AP_hash},
    };

    std::cout << "n";
    for (const auto &name: fns | std::views::keys) std::cout << ',' << name; // for(const auto& [hash_name, hash_function]: fns) is more clear
    std::cout << '\n';

    for (const auto n: ns) {
        std::vector<std::string> strings;
        strings.reserve(n);

        for (auto i = 0uz; i < n; ++i)
            strings.push_back("word_" + std::to_string(i)); // All strings must be unique, they consists of 'a' to 'z', and of the same length 

        std::cout << n;

        for (const auto &fn: fns | std::views::values)
            std::cout << ',' << collisions(strings, fn, table_size);

        std::cout << '\n';
    }
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
// До N = 5 000 большинство функций дают ноль коллизий.
// После N = 20 000 кривые резко расходятся.
//
// Теоретический минимум при M = 100 003 и N = 100 000: ≈ 39 350 коллизий.
//
// Лучшие функции PJW и ELF. До N = 10 000 дают ноль коллизий. При N = 100 000 ближе всего к минимуму.
// Алгоритм выталкивает старшие биты обратно в хеш через XOR, что обеспечивает
// равномерное покрытие пространства значений, в том числе и для монотонных строк.
//
// Худшая функция DJB. Заметно хуже теоретического минимума.
// Формула hash = hash * 33 + c при монотонных строках накапливает одинаковые
// паттерны в старших битах. DJB хорошо работает на случайных ключах, но плохо
// на структурированных. В исследуемом примере строки "word_0", "word_1" отличаются только в последнем
// символе, которому DJB придаёт наименьший вес.
//
// SDBM показывает меньше коллизий, чем остальные методики при N < 50 000, но проигрывает при больших N.
//
// Формула SDBM:
//
//   hash = hash * 65599 + c
//
// Множитель 65599 — простое число с равномерным битовым паттерном, которое
// хорошо распределяет значения при умеренном заполнении таблицы.
//
// При высоком N все функции упираются в физический предел таблицы и начинают
// различаться качеством распределения в старших битах. Здесь SDBM проигрывает.

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
