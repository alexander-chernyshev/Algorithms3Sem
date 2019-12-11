#include <iostream>
#include <vector>
#include <algorithm>

class BinaryIndexedTree {
private:
    size_t width;
    size_t height;
    size_t length;
    std::vector<std::vector<std::vector<int>>> tree;

    void modify(size_t y, size_t x, size_t z, int add) {
        for (size_t i = y; i < height; i = (i | (i + 1))) {
            for (size_t j = x; j < width; j = (j | (j + 1))) {
                for (size_t k = z; k < length; k = (k | (k + 1))) {
                    tree[i][j][k] += add;
                }
            }
        }
    }

    int sum(size_t y, size_t x, size_t z) {
        size_t result = 0;
        for (int i = y; i >= 0; i = (i & (i + 1)) - 1) {
            for (int j = x; j >= 0; j = (j & (j + 1)) - 1) {
                for (int k = z; k >= 0; k = (k & (k + 1)) - 1) {
                    result += tree[i][j][k];
                }
            }
        }
        return result;
    }

public:
    explicit BinaryIndexedTree(size_t _height, size_t _width, size_t _length) {
        width = _width;
        height = _height;
        length = _length;
        tree.resize(height);
        for (auto &v : tree) {
            v.resize(width);
            for (auto &u : v) {
                u.resize(length);
            }
        }
    }

    void Modify(size_t y, size_t x, size_t z, int add) {
        modify(y, x, z, add);
    }

    int GetSum(size_t x1, size_t x2, size_t y1, size_t y2, size_t z1, size_t z2) {
        int result = sum(y2, x2, z2);
        if (x1 > 0) {
            result -= sum(y2, x1 - 1, z2);
        }
        if (y1 > 0) {
            result -= sum(y1 - 1, x2, z2);
        }
        if (z1 > 0) {
            result -= sum(y2, x2, z1 - 1);
        }
        if (x1 > 0 && y1 > 0) {
            result += sum(y1 - 1, x1 - 1, z2);
        }
        if (x1 > 0 && z1 > 0) {
            result += sum(y2, x1 - 1, z1 - 1);
        }
        if (y1 > 0 && z1 > 0) {
            result += sum(y1 - 1, x2, z1 - 1);
        }
        if (y1 > 0 && x1 > 0 && z1 > 0) {
            result -= sum(y1 - 1, x1 - 1, z1 - 1);
        }
        return result;
    }
};

int main() {
    size_t size;
    std::cin >> size;
    BinaryIndexedTree tree(size, size, size);
    size_t query_code = 0;
    do {
        std::cin >> query_code;
        if (query_code == 1) {
            size_t x, y, z;
            int add;
            std::cin >> x >> y >> z >> add;
            tree.Modify(y, x, z, add);
        } else if (query_code == 2) {
            size_t x1, x2, y1, y2, z1, z2;
            std::cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2;
            std::cout << tree.GetSum(x1, x2, y1, y2, z1, z2) << '\n';
        }
    } while (query_code != 3);
    return 0;
}

