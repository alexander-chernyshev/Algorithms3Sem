#include <iostream>
#include <vector>
#include <algorithm>

size_t get_next_array_element(size_t element) {
    return (23 * element + 21563) % 16714589;
}

size_t get_next_left_border(size_t left_border, size_t ans, size_t i, size_t arr_len) {
    return (17 * left_border + 751 + ans + 2 * i) % arr_len + 1;
}

size_t get_next_right_border(size_t right_border, size_t ans, size_t i, size_t arr_len) {
    return (13 * right_border + 593 + ans + 5 * i) % arr_len + 1;
}

std::vector<size_t> GenerateArray(size_t size, size_t element) {
    std::vector<size_t> array(size);
    array[0] = element;
    for (size_t i = 1; i < size; ++i) {
        array[i] = get_next_array_element(array[i - 1]);
    }
    return array;
}

size_t BinaryLog(size_t value) {
    size_t count = 0;
    while (value > 0) {
        value /= 2;
        ++count;
    }
    return count - 1;
}

class SparseTable {
private:
    size_t array_size;
    size_t table_height;
    std::vector<std::vector<size_t>> table;
public:
    explicit SparseTable(const std::vector<size_t> &array) {
        array_size = array.size();
        table_height = BinaryLog(array_size) + 1;
        table.resize(table_height);
        for (auto &v : table) {
            v.resize(array_size);
        }
        for (size_t i = 0; i < array_size; ++i) {
            table[0][i] = array[i];
        }
        for (size_t row = 1; row < table_height; ++row) {
            for (size_t i = 0; i + (1ull << (row - 1)) < array_size; ++i) {
                table[row][i] = std::min(table[row - 1][i], table[row - 1][i + (1ull << (row - 1))]);
            }
        }
    }

    size_t GetMin(size_t l, size_t r) {
        size_t row = BinaryLog(r - l + 1);
        return std::min(table[row][l], table[row][r - (1ull << row) + 1]);
    }
};


int main() {
    size_t n, m, a;
    std::cin >> n >> m >> a;
    std::vector<size_t> arr = GenerateArray(n, a);
    SparseTable st(arr);
    size_t left_border, right_border;
    std::cin >> left_border >> right_border;
    size_t ans = (right_border < left_border ? st.GetMin(right_border - 1, left_border - 1) :
                  st.GetMin(left_border - 1, right_border - 1));
    for (size_t i = 1; i < m; ++i) {
        left_border = get_next_left_border(left_border, ans, i, n);
        right_border = get_next_right_border(right_border, ans, i, n);
        if (right_border < left_border) {
            ans = st.GetMin(right_border - 1, left_border - 1);
        } else {
            ans = st.GetMin(left_border - 1, right_border - 1);
        }
    }
    std::cout << left_border << ' ' << right_border << ' ' << ans;
    return 0;
}