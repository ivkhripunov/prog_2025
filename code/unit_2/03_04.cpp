//
// Created by ivankhripunov on 02.11.2025.
//

#include <gtest/gtest.h>
#include <string>

class Entity {
    std::string line = "test";

    [[nodiscard]] std::string test_v1() const {
        return line + "_v1";
    }

    [[nodiscard]] std::string test_v2() const {
        return line + "_v2";
    }

    friend class Tester_v1;
    friend class Tester_v2;
};

class Tester_v1 {
public:
    static std::string test_v1(const Entity &entity) {
        return entity.test_v1();
    }
};

class Tester_v2 {
public:
    static std::string test_v2(const Entity &entity) {
        return entity.test_v2();
    }
};

TEST(ENTITY, PRIVATE) {
    const Entity entity;

    ASSERT_EQ(Tester_v1::test_v1(entity), "test_v1");
    ASSERT_EQ(Tester_v2::test_v2(entity), "test_v2");
}