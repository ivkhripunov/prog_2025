#include "entity.hpp"

#include <bit>
#include <iostream>
#include <memory>
#include <new>
#include <utility>

class Entity::Implementation {
public :
    explicit Implementation(const int id = 0, const int value = 100)
        : m_id{id}
          , m_value{value} {
        std::cout << "Implementation сконструирован  id=" << m_id
                << "  value=" << m_value << '\n';
    }

    //  -------------------------------------

    Implementation(Implementation &&other) noexcept
        : m_id{other.m_id}
          , m_value{other.m_value} {
        std::cout << "Implementation перемещён  id=" << m_id << '\n';
    }

    //  -------------------------------------

    ~Implementation() {
        std::cout << "Implementation уничтожен  id=" << m_id << '\n';
    }

    //  -------------------------------------

    void test() const {
        std::cout << "Implementation test()  id=" << m_id
                << "  value=" << m_value << '\n';
    }

    //  -------------------------------------

    Implementation &operator=(Implementation &&other) noexcept {
        if (this != &other) {
            m_id = other.m_id;
            m_value = other.m_value;
        }
        return *this;
    }

private :
    int m_id{};
    int m_value{};
};

Entity::Implementation *Entity::get() noexcept {
    return std::launder(
        std::bit_cast<Implementation *>(
            m_storage.data()
        )
    );
}


const Entity::Implementation *Entity::get() const noexcept {
    return std::launder(
        std::bit_cast<const Implementation *>(
            m_storage.data()
        )
    );
}

Entity::Entity() {
    static_assert(
        sizeof(Implementation) <= sizeof (m_storage)
    );

    static_assert(
        alignof(Implementation) <= alignof(std::max_align_t)
    );

    ::new(m_storage.data()) Implementation();
}

Entity::Entity(Entity &&other) noexcept {
    ::new(m_storage.data()) Implementation(std::move(*other.get()));
}

Entity::~Entity() {
    std::destroy_at(get());
}

Entity &Entity::operator=(Entity &&other) noexcept {
    if (this != &other) {
        *get() = std::move(*other.get());
    }
    return *this;
}

void Entity::test() const {
    get()->test();
}

/// Оригинальный паттерн PIMPL заключается в том, что все внутренности прячутся в отдельный класс,
/// хранится только указатель на него. Это позволяет изменть детали реализации внутренностей, при этом
/// не перекомпилируя части проекта, включающие этот заголовочный файл.
/// Накладные расходы оригинального подхода заключаются в следующем.
/// 1. При создании и уничтожении владельца вызывается ресурсоемкий ::operator new / ::operator delete.
///
/// 2. Владелец и Pimpl находятся в разных блоках кучи.
/// Это приводит к кэш-промаху на первых вызовах после конструирования.
///
/// 3. Для доступа к полю требуется одно дополнительное чтения памяти.
/// Из за этого предсказатель ветвлений может работать хуже.

int main() {
    {
        Entity a;
        a.test();
    }

    {
        Entity a;
        Entity b(std::move(a));
        b.test();
    }

    {
        Entity a;
        Entity b;
        b = std::move(a);
        b.test();
    }
}
