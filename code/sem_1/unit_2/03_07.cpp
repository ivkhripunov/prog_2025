//
// Created by ivankhripunov on 01.11.2025.
//

////////////////////////////////////////////////////////////////////////////////////

#include <algorithm>
#include <initializer_list>
#include <iterator>
#include <print>
#include <gtest/gtest.h>
#include <utility>

////////////////////////////////////////////////////////////////////////////////////

class Vector {
public:

    Vector() : m_array(nullptr), m_size(0), m_capacity(0) {
        std::print("Vector:: Vector (1)\n");
    }

//  --------------------------------------------------------------------------------

    Vector(std::initializer_list<int> list) : m_size(std::size(list)), m_capacity(std::size(list)) {
        std::print("Vector:: Vector (2)\n");

        m_array = m_capacity ? new int[m_capacity]{} : nullptr;

        std::ranges::copy(list, m_array);
    }

//  --------------------------------------------------------------------------------

    Vector(Vector const &other) : m_size(other.m_size), m_capacity(other.m_capacity) {
        std::print("Vector:: Vector (3)\n");

        m_array = m_capacity ? new int[m_capacity]{} : nullptr;

        std::ranges::copy(other.m_array, other.m_array + other.m_size, m_array);
    }

//  --------------------------------------------------------------------------------

    Vector(Vector &&other)
            :
            m_array(std::exchange(other.m_array, nullptr)),
            m_size(std::exchange(other.m_size, 0)),
            m_capacity(std::exchange(other.m_capacity, 0)) {
        std::print("Vector:: Vector (4)\n");
    }

//  --------------------------------------------------------------------------------

    ~Vector() {
        std::print("Vector::~Vector\n");

        delete[] m_array;
    }

//  --------------------------------------------------------------------------------

    auto &operator=(Vector other) {
        std::print("Vector::operator= (4)\n");

        swap(other);

        return *this;
    }

//  --------------------------------------------------------------------------------

    void swap(Vector &other) {
        std::swap(m_array, other.m_array);
        std::swap(m_size, other.m_size);
        std::swap(m_capacity, other.m_capacity);
    }

    std::size_t capacity() const {
        return m_capacity;
    }

    std::size_t size() const {
        return m_size;
    }

    bool empty() const {
        return m_size == 0;
    }

    void push_back(int value) {
        if (m_size == m_capacity) {
            reserve(m_capacity == 0 ? 1 : m_capacity * 2);
        }

        m_array[m_size] = value;
        ++m_size;
    }

    void clear() {
        m_size = 0;
    }

    void reserve(std::size_t new_capacity) {
        if (new_capacity <= m_capacity)
            return;

        int *new_array = new int[new_capacity];

        if (m_array) {
            std::ranges::copy(m_array, m_array + m_size, new_array);
            delete[] m_array;
        }

        m_array = new_array;
        m_capacity = new_capacity;
    }

    int &operator[](std::size_t index) {
        return m_array[index];
    }

    const int &operator[](std::size_t index) const {
        return m_array[index];
    }

private:

    int *m_array = nullptr;
    std::size_t m_size = 0;
    std::size_t m_capacity = 0;
};

////////////////////////////////////////////////////////////////////////////////////

void swap(Vector &lhs, Vector &rhs) {
    lhs.swap(rhs);
}

////////////////////////////////////////////////////////////////////////////////////

TEST(TASK_03_07, BASIC) {
    Vector vector_1;
    EXPECT_TRUE(vector_1.empty());
    EXPECT_EQ(vector_1.size(), 0);
    EXPECT_EQ(vector_1.capacity(), 0);

    Vector vector_2 = {1, 2, 3, 4, 5};
    EXPECT_FALSE(vector_2.empty());
    EXPECT_EQ(vector_2.size(), 5);
    EXPECT_GE(vector_2.capacity(), 5);

    Vector vector_3 = vector_2;
    EXPECT_EQ(vector_3.size(), 5);
    EXPECT_EQ(vector_3.capacity(), 5);

    Vector vector_4 = std::move(vector_3);
    EXPECT_EQ(vector_4.size(), 5);
    EXPECT_EQ(vector_3.size(), 0);
    EXPECT_EQ(vector_3.capacity(), 0);
}

TEST(TASK_03_07, PUSH_BACK) {
    Vector vec;

    EXPECT_TRUE(vec.empty());
    EXPECT_EQ(vec.size(), 0);
    EXPECT_EQ(vec.capacity(), 0);

    vec.push_back(10);
    EXPECT_FALSE(vec.empty());
    EXPECT_EQ(vec.size(), 1);
    EXPECT_GE(vec.capacity(), 1);
    EXPECT_EQ(vec[0], 10);

    vec.push_back(20);
    vec.push_back(30);
    EXPECT_EQ(vec.size(), 3);
    EXPECT_GE(vec.capacity(), 3);
    EXPECT_EQ(vec[0], 10);
    EXPECT_EQ(vec[1], 20);
    EXPECT_EQ(vec[2], 30);
}

TEST(TASK_03_07, CAPACITY_GROWTH) {
    Vector vec;

    std::size_t previous_capacity = vec.capacity();

    for (int i = 0; i < 10; ++i) {
        vec.push_back(i);

        if (vec.size() > previous_capacity) {
            EXPECT_GT(vec.capacity(), previous_capacity);
            previous_capacity = vec.capacity();
        }
    }

    EXPECT_EQ(vec.size(), 10);
    EXPECT_GE(vec.capacity(), 10);
}

TEST(TASK_03_07, CLEAR) {
    Vector vec = {1, 2, 3, 4, 5};

    std::size_t capacity_before_clear = vec.capacity();
    EXPECT_FALSE(vec.empty());
    EXPECT_EQ(vec.size(), 5);

    vec.clear();

    EXPECT_TRUE(vec.empty());
    EXPECT_EQ(vec.size(), 0);
    EXPECT_EQ(vec.capacity(), capacity_before_clear);

    vec.push_back(100);
    EXPECT_FALSE(vec.empty());
    EXPECT_EQ(vec.size(), 1);
    EXPECT_EQ(vec.capacity(), capacity_before_clear);
}

TEST(TASK_03_07, VECTOR_RESERVE) {
    Vector vec;

    vec.reserve(10);
    EXPECT_EQ(vec.capacity(), 10);
    EXPECT_EQ(vec.size(), 0);
    EXPECT_TRUE(vec.empty());

    for (int i = 0; i < 10; ++i) {
        vec.push_back(i);
    }

    EXPECT_EQ(vec.size(), 10);
    EXPECT_EQ(vec.capacity(), 10);

    vec.push_back(100);
    EXPECT_EQ(vec.size(), 11);
    EXPECT_GT(vec.capacity(), 10);
}

////////////////////////////////////////////////////////////////////////////////////