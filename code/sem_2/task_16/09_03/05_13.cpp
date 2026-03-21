/////////////////////////////////////////////////

// chapter : Software Design Patterns

/////////////////////////////////////////////////

// section : Behavioral Patterns

/////////////////////////////////////////////////

// content : Pattern Observer

/////////////////////////////////////////////////

#include <print>
#include <vector>
#include <memory>

/////////////////////////////////////////////////

class Observer {
public :
    virtual ~Observer() = default;

    //  -----------------------------------

    virtual void test(int x) const = 0;
};

/////////////////////////////////////////////////

class Entity {
public :
    void add(std::shared_ptr<Observer> observer) {
        m_observers.push_back(std::move(observer));
    }

    //  ---------------------------------------

    void set(int x) {
        m_x = x;

        notify_all();
    }

    //  ---------------------------------------

    void notify_all() const {
        for (const auto &observer: m_observers) {
            if (observer) observer->test(m_x);
        }
    }

private :
    int m_x = 0;

    std::vector<std::shared_ptr<Observer> > m_observers;
};

/////////////////////////////////////////////////

class Client : public Observer {
public :
    void test(int x) const override {
        std::print("Client::test : x = {}\n", x);
    }
};

/////////////////////////////////////////////////

class Server : public Observer {
public :
    void test(int x) const override {
        std::print("Server::test : x = {}\n", x);
    }
};

/////////////////////////////////////////////////

int main() {
    Entity entity;

    entity.add(std::make_shared<Client>());
    entity.add(std::make_shared<Server>());

    for (auto i = 0; i < 2; ++i) {
        entity.set(i + 1);
    }
}

// В этом примере использовался shared_ptr.
// Observer концептуально может быть приписан сразу на несколько Entity.
// shared_ptr позволят реализовать разделённое владение.
// Несколько Entity могут совместно держать один Observer,
// и он будет уничтожен лишь когда последний владелец освободит его.

/////////////////////////////////////////////////
