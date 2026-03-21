///////////////////////////////////////////////////////////

// chapter : Software Design Patterns

///////////////////////////////////////////////////////////

// section : Generative Patterns

///////////////////////////////////////////////////////////

// content : Pattern Prototype
//
// content : Virtual Constructors

///////////////////////////////////////////////////////////

#include <vector>
#include <memory>

///////////////////////////////////////////////////////////

class Entity {
public :
    virtual ~Entity() = default;

    //  ----------------------------------

    [[nodiscard]] virtual std::unique_ptr<Entity> copy() const = 0;
};

///////////////////////////////////////////////////////////

class Client : public Entity {
public :
    [[nodiscard]] std::unique_ptr<Entity> copy() const override {
        return std::make_unique<Client>(*this);
    }
};

///////////////////////////////////////////////////////////

class Server : public Entity {
public :
    [[nodiscard]] std::unique_ptr<Entity> copy() const override {
        return std::make_unique<Server>(*this);
    }
};

///////////////////////////////////////////////////////////

class Prototype {
public:
    Prototype() {
        m_entities.push_back(std::make_unique<Client>());
        m_entities.push_back(std::make_unique<Server>());
    }

    [[nodiscard]] std::unique_ptr<Entity> make_client() const { return m_entities.at(0)->copy(); }
    [[nodiscard]] std::unique_ptr<Entity> make_server() const { return m_entities.at(1)->copy(); }

private:
    std::vector<std::unique_ptr<Entity> > m_entities;
};

///////////////////////////////////////////////////////////

int main() {
    const Prototype prototype;
    const auto client = prototype.make_client();
    const auto server = prototype.make_server();
}

// В этом примере также использовался unique_ptr.
// make_entity() создаёт объект и передаёт владение вызывающему коду.
// Деструктор для элементов вектора прототипов больше не нужен.

///////////////////////////////////////////////////////////
