#include <iostream>

#include <tree.hpp>

int main() {
    Tree tree;

    // Структура дерева:
    //       1
    //     2   3
    //    4 5 6 7

    tree.root = Tree::make_node(1);
    tree.root->left = Tree::make_node(2, tree.root);
    tree.root->right = Tree::make_node(3, tree.root);
    tree.root->left->left = Tree::make_node(4, tree.root->left);
    tree.root->left->right = Tree::make_node(5, tree.root->left);
    tree.root->right->left = Tree::make_node(6, tree.root->right);
    tree.root->right->right = Tree::make_node(7, tree.root->right);

    tree.traverse_v1();
    tree.traverse_v2();

    for (const auto &node: {
             tree.root->left,
             tree.root->right,
             tree.root->left->left,
             tree.root->left->right,
             tree.root->right->left,
             tree.root->right->right
         }) {
        if (const auto p = node->parent.lock())
            std::cout << "Node " << node->value
                    << " -> parent " << p->value << '\n';
    }
}


// Score is 9/10
