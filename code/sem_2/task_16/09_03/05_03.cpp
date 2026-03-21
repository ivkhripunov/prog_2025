/////////////////////////////////////////////

// chapter : Software Design Patterns

/////////////////////////////////////////////

// section : Generative Patterns

/////////////////////////////////////////////

// content : Pattern Abstract Factory

/////////////////////////////////////////////

#include <memory>

class Entity {
public :
    virtual ~Entity() = default;
};

/////////////////////////////////////////////

class Client : public Entity {
};

class Server : public Entity {
};

/////////////////////////////////////////////

class Factory {
public :
    virtual ~Factory() = default;

    //  -----------------------------------------

    [[nodiscard]] virtual std::unique_ptr<Entity> make_entity() const = 0;
};

/////////////////////////////////////////////

class Factory_Client : public Factory {
public :
    [[nodiscard]] std::unique_ptr<Entity> make_entity() const override {
        return std::make_unique<Client>();
    }
};

/////////////////////////////////////////////

class Factory_Server : public Factory {
public :
    [[nodiscard]] std::unique_ptr<Entity> make_entity() const override {
        return std::make_unique<Server>();
    }
};

/////////////////////////////////////////////

int main() {
    const auto factory = std::make_unique<Factory_Client>();
    const auto entity = factory->make_entity();
}

// В этом примере также использовался unique_ptr.
// make_entity() создаёт объект и передаёт владение вызывающему коду.
// Factory создаётся через make_unique в main.

/////////////////////////////////////////////
