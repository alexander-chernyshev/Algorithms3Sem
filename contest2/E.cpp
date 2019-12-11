#include <iostream>
#include <vector>
#include <algorithm>
#include <cinttypes>
#include <utility>

template<class T>
class SegmentTree {
private:
    size_t base_size;
    size_t buffer_size;
    std::vector<T> array;
    std::vector<T> max_element;
    std::vector<size_t> max_count;

    static size_t left_son(size_t node) {
        return node * 2 + 1;
    }

    static size_t right_son(size_t node) {
        return node * 2 + 2;
    }


    void build(size_t node, size_t left, size_t right) {
        if (left == right) {
            max_element[node] = array[left];
            max_count[node] = 1;
        } else {
            size_t mid = (left + right) / 2;
            build(left_son(node), left, mid);
            build(right_son(node), mid + 1, right);
            if (max_element[left_son(node)] == max_element[right_son(node)]) {
                max_element[node] = max_element[left_son(node)];
                max_count[node] = max_count[left_son(node)] + max_count[right_son(node)];
            } else {
                if (max_element[left_son(node)] > max_element[right_son(node)]) {
                    max_element[node] = max_element[left_son(node)];
                    max_count[node] = max_count[left_son(node)];
                } else {
                    max_element[node] = max_element[right_son(node)];
                    max_count[node] = max_count[right_son(node)];
                }
            }
        }
    }

    std::pair<T, size_t> get_max(size_t node, size_t t_left, size_t t_right, size_t left, size_t right) {
        if (left == t_left && right == t_right) {
            return {max_element[node], max_count[node]};
        } else {
            size_t t_mid = (t_left + t_right) / 2;
            if (right <= t_mid) {
                return get_max(left_son(node), t_left, t_mid, left, right);
            } else if (t_mid + 1 <= left) {
                return get_max(right_son(node), t_mid + 1, t_right, left, right);
            }
            std::pair<T, size_t> left_max = get_max(left_son(node), t_left, t_mid, left, t_mid);
            std::pair<T, size_t> right_max = get_max(right_son(node), t_mid + 1, t_right, t_mid + 1, right);
            if (left_max.first == right_max.first) {
                return {left_max.first, left_max.second + right_max.second};
            } else {
                if (left_max.first > right_max.first) {
                    return left_max;
                } else {
                    return right_max;
                }
            }
        }
    }

public:
    explicit SegmentTree(size_t size) {
        base_size = 1;
        while (base_size < size) {
            base_size *= 2;
        }
        buffer_size = base_size * 2 - 1;
        max_element.resize(buffer_size);
        max_count.resize(buffer_size);
        array.resize(base_size);
    }

    explicit SegmentTree(const std::vector<T> &_array) {
        base_size = 1;
        while (base_size < _array.size()) {
            base_size *= 2;
        }
        array = _array;
        buffer_size = base_size * 2 - 1;
        max_element.resize(buffer_size);
        max_count.resize(buffer_size);
        array.resize(base_size);
        build(0, 0, base_size - 1);
    }

    std::pair<T, size_t> GetMaximumCount(size_t left, size_t right) {
        return get_max(0, 0, base_size - 1, left, right);
    }
};

int main() {
    size_t array_size;
    std::cin >> array_size;
    std::vector<size_t> array(array_size);
    for (auto &x : array) {
        std::cin >> x;
    }
    SegmentTree<size_t> tree(array);
    size_t query_count;
    std::cin >> query_count;
    for (size_t i = 0; i < query_count; ++i) {
        size_t left, right;
        std::cin >> left >> right;
        std::pair<size_t, size_t> max = tree.GetMaximumCount(left - 1, right - 1);
        std::cout << max.first << ' ' << max.second << '\n';
    }
    return 0;
}