#pragma once

#include <queue>
#include <stack>
#include <memory>
#include <iostream>

class Tree {
public:
    struct Node {
        int value;
        std::weak_ptr<Node> parent;
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;

        explicit Node(const int v) : value(v) {
            std::cout << "Node constructed: " << value << '\n';
        }

        ~Node() {
            std::cout << "Node destroyed: " << value << '\n';
        }
    };

    std::shared_ptr<Node> root;

    void traverse_v1() const {
        if (!root) return;

        std::cout << "V1: ";
        std::queue<std::shared_ptr<Node> > q;
        q.push(root);

        while (!q.empty()) {
            const auto node = q.front();
            q.pop();
            std::cout << node->value << ' ';
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
        std::cout << '\n';
    }

    void traverse_v2() const {
        if (!root) return;

        std::cout << "V2: ";
        std::stack<std::shared_ptr<Node> > s;
        s.push(root);

        while (!s.empty()) {
            const auto node = s.top();
            s.pop();
            std::cout << node->value << ' ';
            if (node->right) s.push(node->right);
            if (node->left) s.push(node->left);
        }
        std::cout << '\n';
    }

    static std::shared_ptr<Node> make_node(
        const int value,
        const std::shared_ptr<Node> &parent = nullptr) {
        auto node = std::make_shared<Node>(value);
        node->parent = parent;
        return node;
    }
};
