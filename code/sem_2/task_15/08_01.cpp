//
// Created by ivan on 08.03.2026.
//

#include <gtest/gtest.h>
#include <cstring>

class Entity_v1 {
    int v1_;

public:
    Entity_v1(const int v1) : v1_(v1) {
    }

    [[nodiscard]] int v1() const { return v1_; }
};

class Entity_v2 {
public:
    int v2;
};

// 1 способ
TEST(task_08_01, reinterpret) {
    Entity_v1 entity_v1(10);
    ASSERT_EQ(entity_v1.v1(), 10);

    Entity_v2 &entity_v2 = reinterpret_cast<Entity_v2 &>(entity_v1);
    entity_v2.v2 = 100;

    ASSERT_EQ(entity_v1.v1(), 100);
}

// 2 способ
TEST(task_08_01, memory) {
    Entity_v1 entity_v1(10);
    ASSERT_EQ(entity_v1.v1(), 10);

    const int new_v1 = 100;
    std::memcpy(&entity_v1, &new_v1, sizeof(int));

    ASSERT_EQ(entity_v1.v1(), 100);
}
