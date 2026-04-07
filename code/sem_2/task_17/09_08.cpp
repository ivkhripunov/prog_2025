#include <cstddef>
#include <new>
#include <print>

class Entity {
public :
    auto operator new(const std::size_t size) -> void * {
        std::print("Entity::operator new\n");

        return ::operator new(size);
    }

    void operator delete(void *ptr) {
        std::print("Entity::operator delete\n");

        ::operator delete(ptr);
    }

    auto operator new[](const std::size_t size) -> void * {
        std::print("Entity::operator new[]\n");

        return ::operator new[](size);
    }

    void operator delete[](void *ptr) {
        std::print("Entity::operator delete[]\n");

        ::operator delete[](ptr);
    }

    auto operator new(const std::size_t size,
                      const std::nothrow_t &) noexcept -> void * {
        std::print("Entity::operator new nothrow\n");

        return ::operator new(size, std::nothrow);
    }

    void operator delete(void *ptr,
                         const std::nothrow_t &) noexcept {
        std::print("Entity::operator delete nothrow\n");

        ::operator delete(ptr, std::nothrow);
    }

    auto operator new[](const std::size_t size,
                        const std::nothrow_t &) noexcept -> void * {
        std::print("Entity::operator new[] nothrow\n");

        return ::operator new[](size, std::nothrow);
    }

    void operator delete[](void *ptr,
                           const std::nothrow_t &) noexcept {
        std::print("Entity::operator delete[] nothrow\n");

        ::operator delete[](ptr, std::nothrow);
    }

protected :
    Entity() = default;
};

class Client : Entity {
public :
    Client() { std::print("Client:: Client\n"); }

    ~Client() { std::print("Client::~Client\n"); }

    using Entity::operator new;
    using Entity::operator delete;

    using Entity::operator new[];
    using Entity::operator delete[];
};

int main() {
    delete new Client;

    delete[] new Client[3];

    delete new(std::nothrow) Client;

    delete[] new(std::nothrow) Client[3];
}
