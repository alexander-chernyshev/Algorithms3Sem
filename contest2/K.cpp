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

template<class T>
struct AVLNode {
    T value;
    size_t height;
    size_t tree_size;
    AVLNode<T> *left = nullptr;
    AVLNode<T> *right = nullptr;

    explicit AVLNode(T val, size_t _tree_size = 1, size_t h = 1) : value(val), tree_size(_tree_size), height(h) {}

};

template<class T>
class AVLTree {
private:
    AVLNode<T> *root = nullptr;

    static size_t height(AVLNode<T> *node) {
        return (node ? node->height : 0);
    }

    static size_t subtree_size(AVLNode<T> *node) {
        return (node ? node->tree_size : 0);
    }

    static int balance_factor(AVLNode<T> *node) {
        return height(node->right) - height(node->left);
    }

    static void fix_height(AVLNode<T> *node) {
        size_t left_h = height(node->left);
        size_t right_h = height(node->right);
        node->height = std::max(left_h, right_h) + 1;
    }

    static void fix_subtree_size(AVLNode<T> *node) {
        size_t left_size = subtree_size(node->left);
        size_t right_size = subtree_size(node->right);
        node->tree_size = left_size + right_size + 1;
    }

    AVLNode<T> *rotate_right(AVLNode<T> *node) {
        AVLNode<T> *moved_node = node->left;
        node->left = moved_node->right;
        moved_node->right = node;
        fix_height(node);
        fix_height(moved_node);
        fix_subtree_size(node);
        fix_subtree_size(moved_node);
        return moved_node;
    }

    AVLNode<T> *rotate_left(AVLNode<T> *node) {
        AVLNode<T> *moved_node = node->right;
        node->right = moved_node->left;
        moved_node->left = node;
        fix_height(node);
        fix_height(moved_node);
        fix_subtree_size(node);
        fix_subtree_size(moved_node);
        return moved_node;
    }

    AVLNode<T> *balance(AVLNode<T> *node) {
        fix_height(node);
        fix_subtree_size(node);
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
            return new AVLNode<T>(value);
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

    AVLNode<T> *get_kth_element(AVLNode<T> *node, size_t k) {
        if (node == nullptr) {
            return nullptr;
        };
        if (subtree_size(node->left) + 1 == k) {
            return node;
        } else if (subtree_size(node->left) + 1 > k) {
            return get_kth_element(node->left, k);
        } else {
            return get_kth_element(node->right, k - subtree_size(node->left) - 1);
        }
    }

    void next(AVLNode<T> *node, T value, std::pair<T, bool> &result) {
        if (node == nullptr) {
            return;
        }
        next(node->left, value, result);
        if (node->value > value && !result.second) {
            result.first = node->value;
            result.second = true;
        }
        next(node->right, value, result);
    }

    void previous(AVLNode<T> *node, T value, std::pair<T, bool> &result) {
        if (node == nullptr) {
            return;
        }
        previous(node->right, value, result);
        if (node->value < value && !result.second) {
            result.first = node->value;
            result.second = true;
        }
        previous(node->left, value, result);
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

    std::pair<T, bool> GetKthElementValue(size_t k) {
        AVLNode<T> *node = get_kth_element(root, k);
        if (node == nullptr) {
            return {T(0), false};
        } else {
            return {node->value, true};
        }
    }

    std::pair<T, bool> Next(T value) {
        std::pair<T, bool> result(0, false);
        next(root, value, result);
        return result;
    }

    std::pair<T, bool> Previous(T value) {
        std::pair<T, bool> result(0, false);
        previous(root, value, result);
        return result;
    }
};

template<class T>
void Execute(AVLTree<T> &tree) {
    std::string command;
    while (std::cin >> command) {
        if (command == "insert") {
            T value;
            std::cin >> value;
            tree.Insert(value);
        } else if (command == "delete") {
            T value;
            std::cin >> value;
            tree.Remove(value);
        } else if (command == "exists") {
            T value;
            std::cin >> value;
            if (tree.Exists(value)) {
                std::cout << "true\n";
            } else {
                std::cout << "false\n";
            }
        } else if (command == "next") {
            T value;
            std::cin >> value;
            std::pair<T, bool> result = tree.Next(value);
            if (!result.second) {
                std::cout << "none\n";
            } else {
                std::cout << result.first << '\n';
            }
        } else if (command == "prev") {
            T value;
            std::cin >> value;
            std::pair<T, bool> result = tree.Previous(value);
            if (!result.second) {
                std::cout << "none\n";
            } else {
                std::cout << result.first << '\n';
            }
        } else if (command == "kth") {
            size_t k;
            std::cin >> k;
            std::pair<T, bool> result = tree.GetKthElementValue(k);
            if (!result.second) {
                std::cout << "none\n";
            } else {
                std::cout << result.first << '\n';
            }
        }
    }
}

int main() {
    AVLTree<long long int> tree;
    Execute(tree);
    return 0;
}