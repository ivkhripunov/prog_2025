//
// Created by ivankhripunov on 02.11.2025.
//

#include <iostream>
#include <gtest/gtest.h>

class List {
private:
    struct Node {
        int value;
        Node* next;

        Node(int val, Node* nxt = nullptr) : value(val), next(nxt) {}
    };

    Node* head = nullptr;
    Node* tail = nullptr;

public:
    ~List() {
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
    }

    [[nodiscard]] bool empty() const {
        return head == nullptr;
    }

    void show() const {
        Node* current = head;
        while (current != nullptr) {
            std::cout << current->value << " ";
            current = current->next;
        }
        std::cout << std::endl;
    }

    void push_front(int value) {
        Node* new_node = new Node(value, head);
        head = new_node;
        if (tail == nullptr) {
            tail = head;
        }
    }

    void push_back(int value) {
        Node* new_node = new Node(value);
        if (empty()) {
            head = tail = new_node;
        } else {
            tail->next = new_node;
            tail = new_node;
        }
    }

    void pop_front() {
        if (empty()) {
            return;
        }

        Node* temp = head;
        head = head->next;
        delete temp;

        if (head == nullptr) {
            tail = nullptr;
        }
    }

    void pop_back() {
        if (empty()) {
            return;
        }

        if (head == tail) {
            delete head;
            head = tail = nullptr;
            return;
        }

        Node* current = head;
        while (current->next != tail) {
            current = current->next;
        }

        delete tail;
        tail = current;
        tail->next = nullptr;
    }

    [[nodiscard]] int get() const {
        if (empty()) {
            throw std::runtime_error("empty");
        }

        Node* slow = head;
        Node* fast = head;

        while (fast->next != nullptr && fast->next->next != nullptr) {
            slow = slow->next;
            fast = fast->next->next;
        }

        return slow->value;
    }
};

TEST(LIST, EMPTY) {
    List list;
    ASSERT_TRUE(list.empty());

    ASSERT_THROW(list.get(), std::runtime_error);

    ASSERT_NO_THROW(list.pop_front());
    ASSERT_NO_THROW(list.pop_back());
}

TEST(LIST, PUSH_POP_FRONT) {
    List list;

    list.push_front(1);
    ASSERT_FALSE(list.empty());

    list.push_front(2);
    list.push_front(3);

    list.pop_front();
    list.pop_front();
    list.pop_front();

    ASSERT_TRUE(list.empty());
}

TEST(LIST, PUSH_POP_BACK) {
    List list;

    list.push_back(1);
    ASSERT_FALSE(list.empty());

    list.push_back(2);
    list.push_back(3);

    list.pop_back();
    list.pop_back();
    list.pop_back();

    ASSERT_TRUE(list.empty());
}

TEST(LIST, OPERATIONS) {
    List list;

    list.push_front(2);
    list.push_back(3);
    list.push_front(1);

    list.pop_front();
    list.pop_back();

    ASSERT_FALSE(list.empty());
}

TEST(LIST, MIDDLE_ELEMENT) {
    List list;

    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    ASSERT_EQ(list.get(), 2);

    list.push_back(4);
    ASSERT_EQ(list.get(), 2);

    list.pop_back();
    list.pop_back();
    list.pop_back();
    list.push_back(1);
    ASSERT_EQ(list.get(), 1);
}

TEST(LIST, SHOW) {
    List list;

    list.push_back(1);
    list.push_back(2);
    list.push_back(3);

    ASSERT_NO_THROW(list.show());
}

TEST(LIST, MEMORY) {
    List* list = new List;

    list->push_back(1);
    list->push_back(2);
    list->push_back(3);

    ASSERT_NO_THROW(delete list);
}

TEST(LIST, EDGE_CASE) {
    List list;

    list.push_front(1);
    ASSERT_EQ(list.get(), 1);
    list.pop_front();
    ASSERT_TRUE(list.empty());

    list.push_back(1);
    ASSERT_EQ(list.get(), 1);
    list.pop_back();
    ASSERT_TRUE(list.empty());

    list.push_front(1);
    list.push_back(2);
    list.pop_front();
    ASSERT_FALSE(list.empty());
    list.pop_back();
    ASSERT_TRUE(list.empty());
}

TEST(LIST, INTEGRATION) {
    List list;

    ASSERT_TRUE(list.empty());

    list.push_front(10);
    list.push_back(20);
    list.push_front(5);
    list.push_back(30);
    list.push_front(1);

    ASSERT_EQ(list.get(), 10);

    list.pop_front();
    list.pop_back();

    ASSERT_EQ(list.get(), 10);

    list.pop_front();
    list.pop_front();
    list.pop_front();

    ASSERT_TRUE(list.empty());

    list.push_back(100);

    list.push_front(50);

    ASSERT_FALSE(list.empty());
    ASSERT_EQ(list.get(), 50);
}
