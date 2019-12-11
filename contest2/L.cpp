#include <iostream>
#include <vector>
#include <algorithm>
#include <cinttypes>
#include <string>
#include <cstring>
#include <utility>
#include <set>
#include <cstdlib>
#include <ctime>

const size_t MODULE =1000000000;
template<class T>
struct AVLNode {
    T value;
    size_t height;
    T tree_sum;
    AVLNode<T> *left = nullptr;
    AVLNode<T> *right = nullptr;

    explicit AVLNode(T val, T _tree_sum = 0, size_t h = 1) : tree_sum(_tree_sum), value(val), height(h) {}

};

template<class T>
class AVLTree {
private:
    AVLNode<T> *root = nullptr;

    static size_t height(AVLNode<T> *node) {
        return (node ? node->height : 0);
    }

    static T subtree_sum(AVLNode<T>* node) {
        return (node ? node->tree_sum : 0);
    }

    static int balance_factor(AVLNode<T> *node) {
        return height(node->right) - height(node->left);
    }

    static void fix_height(AVLNode<T> *node) {
        size_t left_h = height(node->left);
        size_t right_h = height(node->right);
        node->height = std::max(left_h, right_h) + 1;
    }

    static void fix_subtree_sum(AVLNode<T> *node) {
        T left_sum = subtree_sum(node->left);
        T right_sum = subtree_sum(node->right);
        node->tree_sum = left_sum + right_sum + node->value;
    }

    AVLNode<T> *rotate_right(AVLNode<T> *node) {
        AVLNode<T> *moved_node = node->left;
        node->left = moved_node->right;
        moved_node->right = node;
        fix_height(node);
        fix_height(moved_node);
        fix_subtree_sum(node);
        fix_subtree_sum(moved_node);
        return moved_node;
    }

    AVLNode<T> *rotate_left(AVLNode<T> *node) {
        AVLNode<T> *moved_node = node->right;
        node->right = moved_node->left;
        moved_node->left = node;
        fix_height(node);
        fix_height(moved_node);
        fix_subtree_sum(node);
        fix_subtree_sum(moved_node);
        return moved_node;
    }

    AVLNode<T> *balance(AVLNode<T> *node) {
        fix_height(node);
        fix_subtree_sum(node);
        if (balance_factor(node) == 2) {
            if (balance_factor(node->right) < 0) {
                node->right = rotate_right(node->right);
            }
            return rotate_left(node);
        }
        if (balance_factor(node) == -2) {
            if (balance_factor(node->left) > 0) {
                node->left = rotate_left(node->left);
            }
            return rotate_right(node);
        }
        return node;
    }

    AVLNode<T> *insert(AVLNode<T> *node, T value) {
        if (node == nullptr) {
            return new AVLNode<T>(value, value);
        }
        if (value < node->value) {
            node->left = insert(node->left, value);
        } else {
            node->right = insert(node->right, value);
        }
        return balance(node);
    }

    AVLNode<T> *find_min(AVLNode<T> *node) {
        return node->left ? find_min(node->left) : node;
    }

    AVLNode<T> *remove_min(AVLNode<T> *node) {
        if (node->left == nullptr) {
            return node->right;
        }
        node->left = remove_min(node->left);
        return balance(node);
    }

    AVLNode<T> *remove(AVLNode<T> *node, T value) {
        if (node == nullptr) {
            return nullptr;
        }
        if (value < node->value) {
            node->left = remove(node->left, value);
        } else if (value > node->value) {
            node->right = remove(node->right, value);
        } else {
            AVLNode<T> *l = node->left;
            AVLNode<T> *r = node->right;
            delete node;
            if (r == nullptr) {
                return l;
            }
            AVLNode<T> *min = find_min(r);
            min->right = remove_min(r);
            min->left = l;
            return balance(min);
        }
        return balance(node);
    }

    T get_lower_sum(AVLNode<T>* node, T value) {
        if (node == nullptr) {
            return 0;
        }
        if (value <= node->value) {
            return get_lower_sum(node->left, value);
        } else {
            return get_lower_sum(node->right, value) + subtree_sum(node->left) + node->value;
        }
    }

public:
    bool Exists(T value) {
        AVLNode<T> *node = root;
        if (node == nullptr) {
            return false;
        }
        while (node != nullptr) {
            if (value == node->value) {
                return true;
            }
            if (value < node->value) {
                node = node->left;
            } else {
                node = node->right;
            }
        }
        return false;
    }

    void Insert(T value) {
        if (Exists(value)) {
            return;
        }
        root = insert(root, value);
    }

    void Remove(T value) {
        root = remove(root, value);
    }

    T GetLowerSum(T value) {
        return get_lower_sum(root, value);
    }

    T GetSegmentSum(T left, T right) {
        return GetLowerSum(right+1) - GetLowerSum(left);
    }
};

template <class T>
void Execute(AVLTree<T>& tree, size_t query_count) {
    size_t buffer = 0;
    for (size_t i = 0; i < query_count; ++i) {
        char query;
        std::cin >> query;
        if (query == '+') {
            T value;
            std::cin >> value;
            tree.Insert((buffer + value) % MODULE);
            buffer = 0;
        }
        else if (query == '?') {
            T left, right;
            std::cin >> left >> right;
            std::cout << (buffer = tree.GetSegmentSum(left, right)) << '\n';
        }
    }
}

int main() {
    size_t query_count;
    std::cin >> query_count;
    AVLTree<size_t> tree;
    Execute(tree, query_count);
    return 0;
}