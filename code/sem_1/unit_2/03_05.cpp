//
// Created by ivankhripunov on 02.11.2025.
//

#include <string>
#include <gtest/gtest.h>

class Entity_v1 {
public:
    std::string str = "Entity_v1::test";

    [[nodiscard]] virtual std::string test() const {
        return str;
    }
};

class Entity_v2 {
public:
    std::string str = "Entity_v2::test";

    [[nodiscard]] virtual std::string test() const {
        return str;
    }
};

class Adapter_v1 : public Entity_v1 {
public:
    virtual std::string test_v1() {
        return Entity_v1::test();
    }
};

class Adapter_v2 : public Entity_v2 {
public:
    virtual std::string test_v2() {
        return Entity_v2::test();
    }
};

class Client final : public Adapter_v1, public Adapter_v2 {
public:
    std::string test_v1() override final {
        return Adapter_v1::test_v1();
    }

    std::string test_v2() override final {
        return Adapter_v2::test_v2();
    }
};

TEST(INHERITANCE, SIAM) {
    Client client;

    ASSERT_EQ(client.test_v1(), "Entity_v1::test");
    ASSERT_EQ(client.test_v2(), "Entity_v2::test");
}
