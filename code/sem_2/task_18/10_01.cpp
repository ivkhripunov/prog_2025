#include <cstddef>
#include <cstdint>
#include <deque>
#include <print>
#include <vector>

static void vector_capacity() {
    std::vector<int> v;

    std::size_t prev = 0;

    std::print("{:>10}  {:>10}  {:>8}\n", "old", "new", "factor");

    for (auto i = 0; i < 10'000; ++i) {
        v.push_back(i);

        if (v.capacity() != prev) {
            if (prev > 0)
                std::print("{:>10}  {:>10}  {:>8.4f}\n",
                           prev, v.capacity(),
                           static_cast<double>(v.capacity()) / prev);

            prev = v.capacity();
        }
    }
}

static void vector_addresses() {
    std::vector<int> v;

    std::print("{:>8}  {:>18}  {:>18}\n", "size", "&v[0]", "&v[size-1]");

    for (auto i = 0; i < 20; ++i) {
        v.push_back(i);

        std::print("{:>8}  {:#018x}  {:#018x}\n",
                   v.size(),
                   reinterpret_cast<std::uintptr_t>(&v[0]),
                   reinterpret_cast<std::uintptr_t>(&v.back()));
    }
}

static void deque_pages() {
    auto detect = []< typename T >() -> std::size_t {
        std::deque<T> d;

        for (auto i = 0uz; i < 4096; ++i) d.push_back(T{});

        std::vector<std::size_t> b;
        auto prev = reinterpret_cast<std::uintptr_t>(&d[0]);

        for (auto i = 1uz; i < 4096; ++i) {
            auto curr = reinterpret_cast<std::uintptr_t>(&d[i]);
            if (curr - prev != sizeof(T)) b.push_back(i);
            prev = curr;
        }

        return b.size() >= 2 ? b[1] - b[0] : 0;
    };

    std::print("{:>16}  {:>8}  {:>16}  {:>12}\n",
               "type", "sizeof", "elements/page", "bytes/page");

    auto row = [&]< typename T >(const char *name) {
        auto n = detect.operator()<T>();
        std::print("{:>16}  {:>8}  {:>16}  {:>12}\n",
                   name, sizeof(T), n, n * sizeof(T));
    };

    row.operator()<char>("char");
    row.operator()<short>("short");
    row.operator()<int>("int");
    row.operator()<double>("double");
    row.operator()<long double>("long double");
}

static void deque_addresses() {
    std::deque<int> d;

    std::print("{:>8}  {:>18}  {:>18}\n", "size", "&d[0]", "&d[size-1]");

    for (auto i = 0; i < 300; ++i) {
        d.push_back(i);

        std::print("{:>8}  {:#018x}  {:#018x}\n",
                   d.size(),
                   reinterpret_cast<std::uintptr_t>(&d[0]),
                   reinterpret_cast<std::uintptr_t>(&d.back()));
    }
}

int main() {
    std::print("vector capacity:\n");
    vector_capacity();

    std::print("Vector element addresses:\n");
    vector_addresses();

    std::print("Deque page size:\n");
    deque_pages();

    std::print("Deque element addresses:\n");
    deque_addresses();
}

/// Проведены тесты для vector и deque.
///
/// Показано, что vector резервирует объем памяти, соответствующий размеру элементов в количестве 2^N, где N = 0, 1...
/// При истощении резерва памяти выделяется новый блок в 2 раза больше предыдущего.
/// При этом старая память освобождается, все данные копируются в новый блок, старые указатели и ссылки умирают.
///
/// Размер страницы deque определялся в результате прохода по всем элементам со сравнением адресов.
/// Если разница между адресами соседних элементов превышает размер типа, то элементы лежат на разных страницах.
/// Установлено, что размер страницы всегда равен 512 байт. При этом указатели на уже добавленные в deque элементы
/// всегда остаются валидными. При заполнении страницы создается следующая страница.
