#include <iostream>
#include <vector>
#include <algorithm>
#include <cinttypes>
#include <utility>
#include <string>

struct segment_info {
    size_t length;
    size_t max_cbs_len;
    size_t left_braces_count;
    size_t right_braces_count;

    segment_info(size_t len, size_t m, size_t l, size_t r) : length(len), max_cbs_len(m), left_braces_count(l),
                                                             right_braces_count(r) {};
};

class SegmentTree {
private:
    size_t base_size;
    size_t buffer_size;
    std::string array;
    std::vector<size_t> max_cbs_len;
    std::vector<size_t> left_braces_count;
    std::vector<size_t> right_braces_count;

    static size_t left_son(size_t node) {
        return node * 2 + 1;
    }

    static size_t right_son(size_t node) {
        return node * 2 + 2;
    }

    void build(size_t node, size_t left, size_t right) {
        if (left == right) {
            if (array[left] == '(') {
                left_braces_count[node] = 1;
            } else if (array[right] == ')') {
                right_braces_count[node] = 1;
            }
            return;
        } else {
            size_t mid = (left + right) / 2;
            build(left_son(node), left, mid);
            build(right_son(node), mid + 1, right);
            size_t min_br_count = std::min(right_braces_count[right_son(node)], left_braces_count[left_son(node)]);
            max_cbs_len[node] = max_cbs_len[left_son(node)] + max_cbs_len[right_son(node)] +
                                2 * min_br_count;
            left_braces_count[node] =
                    left_braces_count[left_son(node)] + left_braces_count[right_son(node)] - min_br_count;
            right_braces_count[node] =
                    right_braces_count[left_son(node)] + right_braces_count[right_son(node)] - min_br_count;
        }
    }

    segment_info get_max_cbs_len(size_t node, size_t t_left, size_t t_right, size_t left, size_t right) {
        if (left == t_left && right == t_right) {
            return {t_right + 1 - t_left, max_cbs_len[node], left_braces_count[node], right_braces_count[node]};
        } else {
            size_t t_mid = (t_left + t_right) / 2;
            if (right <= t_mid) {
                return get_max_cbs_len(left_son(node), t_left, t_mid, left, right);
            } else if (t_mid + 1 <= left) {
                return get_max_cbs_len(right_son(node), t_mid + 1, t_right, left, right);
            }
            segment_info left_segment = get_max_cbs_len(left_son(node), t_left, t_mid, left, t_mid);
            segment_info right_segment = get_max_cbs_len(right_son(node), t_mid + 1, t_right, t_mid + 1, right);
            size_t len = left_segment.length + right_segment.length;
            size_t min_br_cnt = std::min(left_segment.left_braces_count, right_segment.right_braces_count);
            size_t max_cbs_length = left_segment.max_cbs_len + right_segment.max_cbs_len +
                                    2 * min_br_cnt;
            size_t left_braces_cnt = left_segment.left_braces_count + right_segment.left_braces_count - min_br_cnt;
            size_t right_braces_cnt = left_segment.right_braces_count + right_segment.right_braces_count - min_br_cnt;
            return {len, max_cbs_length, left_braces_cnt, right_braces_cnt};
        }
    }

public:
    explicit SegmentTree(size_t
                         size) {
        base_size = 1;
        while (base_size < size) {
            base_size *= 2;
        }
        buffer_size = base_size * 2 - 1;
        max_cbs_len.resize(buffer_size);
        left_braces_count.resize(buffer_size);
        right_braces_count.resize(buffer_size);
        array.resize(base_size);
    }

    explicit SegmentTree(const std::string &_array) {
        base_size = 1;
        while (base_size < _array.size()) {
            base_size *= 2;
        }
        array = _array;
        buffer_size = base_size * 2 - 1;
        max_cbs_len.resize(buffer_size);
        left_braces_count.resize(buffer_size);
        right_braces_count.resize(buffer_size);
        array.resize(base_size, '(');
        build(0, 0, base_size - 1);
    }

    size_t GetMaximumCount(size_t left, size_t right) {
        return get_max_cbs_len(0, 0, base_size - 1, left, right).max_cbs_len;
    }
};

int main() {
    std::string sequence;
    std::cin >> sequence;
    SegmentTree tree(sequence);
    size_t query_count;
    std::cin >> query_count;
    for (size_t i = 0; i < query_count; ++i) {
        size_t left, right;
        std::cin >> left >> right;
        std::cout << tree.GetMaximumCount(left - 1, right - 1) << '\n';
    }
    return 0;
}