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
    std::vector<T> buffer;

    static size_t left_son(size_t node) {
        return node * 2 + 1;
    }

    static size_t right_son(size_t node) {
        return node * 2 + 2;
    }

    void build(size_t node, size_t left, size_t right) {
        if (left == right) {
            buffer[node] = array[left];
        } else {
            size_t mid = (left + right) / 2;
            build(left_son(node), left, mid);
            build(right_son(node), mid + 1, right);
            buffer[node] = buffer[left_son(node)] + buffer[right_son(node)];
        }
    }

    T sum(size_t node, size_t t_left, size_t t_right, size_t left, size_t right) {
        if (left > right) {
            return 0;
        }
        if (left == t_left && right == t_right) {
            return buffer[node];
        } else {
            size_t t_mid = (t_left + t_right) / 2;
            if (right <= t_mid) {
                return sum(left_son(node), t_left, t_mid, left, right);
            } else if (t_mid + 1 <= left) {
                return sum(right_son(node), t_mid + 1, t_right, left, right);
            }
            return sum(left_son(node), t_left, t_mid, left, t_mid) +
                   sum(right_son(node), t_mid + 1, t_right, t_mid + 1, right);
        }
    }

   void update(size_t node, size_t left, size_t right, size_t pos, T value) {
        if (left == right) {
            array[left] = value;
            buffer[node] = value;
        } else {
            size_t mid = (left + right) / 2;
            if (pos <= mid) {
                update(left_son(node), left, mid, pos, value);
            } else {
                update(right_son(node), mid + 1, right, pos, value);
            }
            buffer[node] = buffer[left_son(node)] + buffer[right_son(node)];
        }
    }

public:
    explicit SegmentTree(size_t size) {
        base_size = 1;
        while (base_size < size) {
            base_size *= 2;
        }
        buffer_size = base_size * 2 - 1;
        buffer.resize(buffer_size);
        array.resize(base_size);
    }

    explicit SegmentTree(const std::vector<T> &_array) {
        base_size = 1;
        while (base_size < _array.size()) {
            base_size *= 2;
        }
        array = _array;
        buffer_size = base_size * 2 - 1;
        buffer.resize(buffer_size);
        array.resize(base_size);
        build(0, 0, base_size - 1);
    }

    void Update(size_t pos, T value) {
        update(0, 0, base_size - 1, pos, value);
    }

    T GetSegmentSum(size_t left, size_t right) {
        return sum(0, 0, base_size - 1, left, right);
    }
};

template<class T>
void QueryHandler(SegmentTree<T> &tree, size_t query_count) {
    char query_code;
    for (size_t i = 0; i < query_count; ++i) {
        std::cin >> query_code;
        if (query_code == 's') {
            size_t left, right;
            std::cin >> left >> right;
            std::cout << tree.GetSegmentSum(left - 1, right - 1) << ' ';
        }
        if (query_code == 'u') {
            size_t pos;
            T value;
            std::cin >> pos >> value;
            tree.Update(pos - 1, value);
        }
    }
}

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
    QueryHandler(tree, query_count);
    return 0;
}