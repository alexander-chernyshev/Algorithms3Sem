#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>

size_t BinaryLog(size_t value) {
    size_t count = 0;
    while (value > 0) {
        value /= 2;
        ++count;
    }
    return count - 1;
}

size_t GCD(size_t a, size_t b) {
    if (a < b) {
        std::swap(a, b);
    }
    if (b == 0) {
        return a;
    }
    return GCD(b, a % b);
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
                table[row][i] = GCD(table[row - 1][i], table[row - 1][i + (1ull << (row - 1))]);
            }
        }
    }

    size_t GetGCD(size_t l, size_t r) {
        size_t row = BinaryLog(r - l + 1);
        return GCD(table[row][l], table[row][r - (1ull << row) + 1]);
    }
};


int main() {
    size_t n;
    std::cin >> n;
    std::vector<size_t> array(n);
    for (auto &x : array) {
        std::cin >> x;
    }
    SparseTable table(array);
    size_t query_count;
    std::cin >> query_count;
    for (size_t i = 0; i < query_count; ++i) {
        size_t left, right;
        std::cin >> left >> right;
        std::cout << table.GetGCD(left - 1, right - 1) << '\n';
    }
    return 0;
}