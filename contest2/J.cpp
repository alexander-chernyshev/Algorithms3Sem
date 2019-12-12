#include <iostream>
#include <vector>
#include <algorithm>
#include <cinttypes>
#include <utility>

template<class T>
struct TreapNode {
    size_t index = 0;
    T key = 0;
    int priority = 0;
    TreapNode<T> *left = nullptr;
    TreapNode<T> *right = nullptr;
    TreapNode<T> *parent = nullptr;

    TreapNode<T>() = default;

    TreapNode<T>(size_t _index, T _key, int _priority) : index(_index), key(_key), priority(_priority) {}
};

template<class T>
struct node_priority_cmp {
    bool operator()(const TreapNode<T> &a, const TreapNode<T> &b) {
        return a.priority < b.priority;
    }
};

template<class T>
struct node_key_cmp {
    bool operator()(const TreapNode<T> &a, const TreapNode<T> &b) {
        return a.key < b.key;
    }
};

template<class T>
struct node_info {
    size_t index;
    size_t parent;
    size_t left_son;
    size_t right_son;

    node_info<T>(size_t _i, size_t _p, size_t _l, size_t _r) : index(_i), parent(_p), left_son(_l), right_son(_r) {};

    node_info<T>() = default;

    explicit node_info<T>(TreapNode<T> *node) {
        index = node->index;
        parent = (node->parent ? node->parent->index : 0);
        left_son = (node->left ? node->left->index : 0);
        right_son = (node->right ? node->right->index : 0);
    }
};

template<class T>
struct node_info_cmp {
    bool operator()(const node_info<T> &a, const node_info<T> &b) {
        return a.index < b.index;
    }
};

template<class T>
class Treap {
private:
    TreapNode<T> *root = nullptr;

    static void fix_sons_parent(TreapNode<T> *node) {
        if (node == nullptr) {
            return;
        }
        if (node->left != nullptr) {
            node->left->parent = node;
        }
        if (node->right != nullptr) {
            node->right->parent = node;
        }
    }

    std::pair<TreapNode<T> *, TreapNode<T> *> split(TreapNode<T> *node, T key) {
        if (node == nullptr) {
            return {nullptr, nullptr};
        }
        if (key > node->key) {
            auto p = split(node->right, key);
            node->right = p.first;
            fix_sons_parent(node);
            return {node, p.second};
        } else {
            auto p = split(node->left, key);
            node->left = p.second;
            fix_sons_parent(node);
            return {p.first, node};
        }
    }

    TreapNode<T> *merge(TreapNode<T> *left_node, TreapNode<T> *right_node) {
        if (right_node == nullptr) {
            return left_node;
        }
        if (left_node == nullptr) {
            return right_node;
        }
        if (left_node->priority < right_node->priority) {
            left_node->right = merge(left_node->right, right_node);
            fix_sons_parent(left_node);
            return left_node;
        } else {
            right_node->left = merge(left_node, right_node->left);
            fix_sons_parent(right_node);
            return right_node;
        }
    }

    TreapNode<T> *insert(size_t index, T key, int priority) {
        std::pair<TreapNode<T> *, TreapNode<T> *> tree_parts = split(root, key);
        auto *new_node = new TreapNode<T>(index, key, priority);
        return merge(merge(tree_parts.first, new_node), tree_parts.second);
    }

    TreapNode<T> *insert(TreapNode<T> *node) {
        std::pair<TreapNode<T> *, TreapNode<T> *> tree_parts = split(root, node->key);
        return merge(merge(tree_parts.first, node), tree_parts.second);
    }

public:
    Treap<T>(TreapNode<T> *array, size_t size) {
        std::sort(array, array + size, node_key_cmp<T>());
        TreapNode<T>* right_node = nullptr;
        for (size_t i = 0; i < size; ++i) {
            TreapNode<T>* inserted_node = array + i;
            if (!right_node) {
                root = right_node = inserted_node;
                continue;
            }
            TreapNode<T>* current = right_node;
            while (current && current->priority > inserted_node->priority) {
                current = current->parent;
            }
            if (!current) {
                inserted_node->left = root;
                root = inserted_node;
                fix_sons_parent(root);
            } else {
                inserted_node->left = current->right;
                current->right = inserted_node;
                fix_sons_parent(current);
                fix_sons_parent(inserted_node);
            }
            right_node = inserted_node;
        }
    }

    void Insert(TreapNode<T> *node) {
        root = insert(node);
    }

    node_info<T> GetNodeInfo(TreapNode<T> *node) {
        return node_info<T>(node);
    }
};

int main() {
    size_t count;
    std::cin >> count;
    auto arr = new TreapNode<int>[count];
    for (size_t i = 0; i < count; ++i) {
        std::cin >> arr[i].key >> arr[i].priority;
        arr[i].index = i + 1;
    }
    Treap<int> treap(arr, count);
    std::vector<node_info<int>> nodes_info(count);
    for (size_t i = 0; i < count; ++i) {
        nodes_info[i] = treap.GetNodeInfo(arr + i);
    }
    std::sort(nodes_info.begin(), nodes_info.end(), node_info_cmp<int>());
    std::cout << "YES" << '\n';
    for (auto i : nodes_info) {
        std::cout << i.parent << ' ' << i.left_son << ' ' << i.right_son << '\n';
    }
    delete[] arr;
    return 0;
}