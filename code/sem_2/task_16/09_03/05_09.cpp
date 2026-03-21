///////////////////////////////////////////////////////////////////////

// chapter : Software Design Patterns

///////////////////////////////////////////////////////////////////////

// section : Constructional Patterns

///////////////////////////////////////////////////////////////////////

// content : Pattern Composite

///////////////////////////////////////////////////////////////////////

#include <cassert>
#include <cstddef>
#include <vector>
#include <memory>

///////////////////////////////////////////////////////////////////////

class Entity {
public :
    virtual ~Entity() = default;

    //  -----------------------------

    [[nodiscard]] virtual int test() const = 0;
};

///////////////////////////////////////////////////////////////////////

class Client : public Entity {
public :
    [[nodiscard]] int test() const override {
        return 1;
    }
};

///////////////////////////////////////////////////////////////////////

class Server : public Entity {
public :
    [[nodiscard]] int test() const override {
        return 2;
    }
};

///////////////////////////////////////////////////////////////////////

class Composite : public Entity {
public :
    void add(std::unique_ptr<Entity> entity) {
        m_entities.push_back(std::move(entity));
    }

    //  ------------------------------------

    [[nodiscard]] int test() const override {
        auto x = 0;

        for (const auto &entity: m_entities) {
            if (entity) {
                x += entity->test();
            }
        }

        return x;
    }

private :
    std::vector<std::unique_ptr<Entity> > m_entities;
};

///////////////////////////////////////////////////////////////////////

auto make_composite(const std::size_t size_1, const std::size_t size_2) -> std::unique_ptr<Entity> {
    auto composite = std::make_unique<Composite>();
    for (std::size_t i = 0; i < size_1; ++i) {
        composite->add(std::make_unique<Client>());
    }
    for (std::size_t i = 0; i < size_2; ++i) {
        composite->add(std::make_unique<Server>());
    }
    return composite;
}

///////////////////////////////////////////////////////////////////////

int main() {
    auto composite = std::make_unique<Composite>();

    for (std::size_t i = 0; i < 5; ++i) {
        composite->add(make_composite(1, 1));
    }

    const Entity *entity = composite.get();
    assert(entity->test() == 15);
}

// В этом примере также использовался unique_ptr.
// add() принимает unique_ptr<Entity> по значению и забирает владение через std::move.
// Деструктор ~Composite удалён, так как вектор сам освобождает узлы.

///////////////////////////////////////////////////////////////////////
