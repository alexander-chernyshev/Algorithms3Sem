#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

class DSU {
private:
    size_t size;
    size_t set_count = 0;
    std::vector<size_t> parent;
    std::vector<size_t> rank;
public:
    explicit DSU(size_t _size) : size(_size) {
        parent.resize(size);
        rank.resize(size);
    };

    void MakeSet(size_t v) {
        ++set_count;
        parent[v] = v;
        rank[v] = 0;
    }

    size_t FindSet(size_t v) {
        if (v == parent[v]) {
            return v;
        }
        return parent[v] = FindSet(parent[v]);
    }

    void UnionSets(size_t a, size_t b) {
        a = FindSet(a);
        b = FindSet(b);
        if (a != b) {
            --set_count;
            if (rank[a] < rank[b]) {
                std::swap(a, b);
            }
            parent[b] = a;
            if (rank[a] == rank[b]) {
                ++rank[a];
            }
        }
    }

    size_t GetSetCount() const {
        return set_count;
    }
};

int main() {
    size_t vertex_count, edge_count;
    std::cin >> vertex_count >> edge_count;
    DSU dsu(vertex_count);
    for (size_t i = 0; i < vertex_count; ++i) {
        dsu.MakeSet(i);
    }
    if (dsu.GetSetCount() == 1) {
        std::cout << 0;
    } else {
        for (size_t i = 0; i < edge_count; ++i) {
            size_t from, to;
            std::cin >> from >> to;
            dsu.UnionSets(from, to);
            if (dsu.GetSetCount() == 1) {
                std::cout << i + 1;
                break;
            }
        }
    }
    return 0;
}