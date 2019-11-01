#include <iostream>
#include <vector>


class DSU {
private:
    size_t size;
    size_t set_count = 0;
    std::vector<size_t> parent;
    std::vector<size_t> rank;
    std::vector<size_t> weight;
public:
    explicit DSU(size_t _size) : size(_size) {
        parent.resize(size);
        rank.resize(size);
        weight.resize(size);
    };

    void MakeSet(size_t v) {
        ++set_count;
        parent[v] = v;
        rank[v] = 0;
        weight[v] = 0;
    }

    size_t FindSet(size_t v) {
        if (v == parent[v]) {
            return v;
        }
        return parent[v] = FindSet(parent[v]);
    }

    size_t GetWeight(size_t v) {
        v = FindSet(v);
        return weight[v];
    }

    void UnionSets(size_t a, size_t b, int edge_weight) {
        a = FindSet(a);
        b = FindSet(b);
        if (a != b) {
            --set_count;
            if (rank[a] < rank[b]) {
                std::swap(a, b);
            }
            parent[b] = a;
            weight[a] += weight[b] + edge_weight;
            if (rank[a] == rank[b]) {
                ++rank[a];
            }
        } else {
            weight[a] += edge_weight;
        }
    }

    size_t GetSetCount() const {
        return set_count;
    }

    size_t GetSize() const {
        return size;
    }
};


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    size_t vertex_count, query_count;
    std::cin >> vertex_count >> query_count;
    DSU dsu(vertex_count);
    for (size_t i = 0; i < vertex_count; ++i) {
        dsu.MakeSet(i);
    }
    for (size_t i = 0; i < query_count; ++i) {
        size_t query;
        std::cin >> query;
        if (query == 1) {
            size_t x, y;
            int w;
            std::cin >> x >> y >> w;
            dsu.UnionSets(x - 1, y - 1, w);
        } else if (query == 2) {
            size_t v;
            std::cin >> v;
            std::cout << dsu.GetWeight(v - 1) << '\n';
        }
    }
    return 0;
}