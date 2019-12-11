#include <iostream>
#include <vector>
#include <algorithm>

class BinaryIndexedTree {
private:
    size_t width;
    size_t height;
    std::vector<std::vector<int>> tree;

    void modify(size_t y, size_t x, int add) {
        for (size_t i = y; i < height; i = (i | (i + 1))) {
            for (size_t j = x; j < width; j = (j | (j + 1))) {
                tree[i][j] += add;
            }
        }
    }

    int sum(size_t y, size_t x) {
        size_t result = 0;
        for (int i = y; i >= 0; i = (i & (i + 1)) - 1) {
            for (int j = x; j >= 0; j = (j & (j + 1)) - 1) {
                result += tree[i][j];
            }
        }
        return result;
    }

public:
    explicit BinaryIndexedTree(size_t _height, size_t _width) {
        width = _width;
        height = _height;
        tree.resize(height);
        for (auto& v : tree) {
            v.resize(width);
        }
    }

    void Modify(size_t y, size_t x, int add) {
        modify(y, x, add);
    }

    int GetSum(size_t left, size_t right, size_t up, size_t down) {
        int result = sum(down, right);
        if (left > 0) {
            result -= sum(down, left-1);
        }
        if (up > 0) {
            result -= sum(up-1, right);
        }
        if (left > 0 && up > 0) {
            result += sum(up-1, left-1);
        }
        return result;
    }
};

int main() {
    size_t width, height;
    std::cin >> width >> height;
    BinaryIndexedTree tree(height, width);
    size_t query_count;
    std::cin >> query_count;
    for (size_t k = 0; k < query_count; ++k) {
        size_t query_code;
        std::cin >> query_code;
        if (query_code == 1) {
            size_t x, y;
            int add;
            std::cin >> x >> y >> add;
            tree.Modify(y-1, x-1, add);
        }
        else if (query_code == 2) {
            size_t left, right, up, down;
            std::cin >> left >> up >> right >> down;
            std::cout << tree.GetSum(left-1, right-1, up-1, down-1) << '\n';
        }
    }
    return 0;
}
