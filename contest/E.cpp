#include <iostream>
#include <vector>
#include <algorithm>

class Graph {
protected:
    size_t vertex_count;
    size_t edge_count = 0;
    bool directed;
public:
    typedef size_t Vertex;

    Graph(size_t _vertex_count, bool _directed);

    virtual std::vector<Vertex> GetNeighbours(const Vertex &v) const = 0;

    virtual void AddEdge(const Vertex &from, const Vertex &to) = 0;
    virtual size_t GetIncomeDegree(const Vertex& v) const = 0;
    virtual size_t GetOutcomeDegree(const Vertex& v) const = 0;

    size_t GetVertexCount() const;
};

class AdjListGraph : public Graph {
private:
    std::vector<std::vector<Vertex>> adj_list;
public:
    AdjListGraph(size_t _vertex_count, bool _directed);

    std::vector<Vertex> GetNeighbours(const Vertex &v) const override;
    size_t GetIncomeDegree(const Vertex& v) const override;
    size_t GetOutcomeDegree(const Vertex& v) const override;

    void AddEdge(const Vertex &from, const Vertex &to) override;
};
namespace GraphProcessing {
    const int UNVISITED_VERTEX = -1;

    enum VertexMark {
        WHITE, GREY, BLACK
    };

    void DFS(const Graph &g, std::vector<VertexMark> &color, Graph::Vertex v, std::vector<int>& path,
             std::vector<std::vector<Graph::Vertex>> &cycles) {
        color[v] = GREY;
        for (Graph::Vertex &u : g.GetNeighbours(v)) {
            if (color[u] == WHITE) {
                path[u] = v;
                DFS(g, color, u, path, cycles);
            } else if (color[u] == GREY) {
                std::vector<Graph::Vertex> cycle;
                Graph::Vertex current = v;
                while (current != u) {
                    cycle.push_back(current);
                    current = path[current];
                }
                cycle.push_back(u);
                std::reverse(cycle.begin(), cycle.end());
                cycles.push_back(cycle);
            }
        }
        color[v] = BLACK;
    }

    std::vector<std::vector<Graph::Vertex>> GetCycles(const Graph &g) {
        std::vector<std::vector<Graph::Vertex>> cycles;
        std::vector<VertexMark> color(g.GetVertexCount(), WHITE);
        std::vector<int> path(g.GetVertexCount(), UNVISITED_VERTEX);
        for (Graph::Vertex v = 0; v < g.GetVertexCount(); ++v) {
            if (color[v] == WHITE) {
                DFS(g, color, v, path, cycles);
            }
        }
        return cycles;
    }

}

int main() {
    size_t vertex_count;
    std::cin >> vertex_count;
    AdjListGraph g(vertex_count, true);
    for (size_t i = 0; i < vertex_count; ++i) {
        size_t vert;
        std::cin >> vert;
        g.AddEdge(vert-1, i);
    }
    size_t piggy_banks = 0;
    std::vector<std::vector<Graph::Vertex>> cycles = GraphProcessing::GetCycles(g);
    piggy_banks += cycles.size();
    for (Graph::Vertex v = 0; v < g.GetVertexCount(); ++v) {
        if (g.GetIncomeDegree(v) == 0) {
            ++piggy_banks;
        }
    }
    std::cout << piggy_banks << std::endl;
    return 0;
}

Graph::Graph(size_t _vertex_count, bool _directed) {
    vertex_count = _vertex_count;
    directed = _directed;
}

size_t Graph::GetVertexCount() const {
    return vertex_count;
}

AdjListGraph::AdjListGraph(size_t _vertex_count, bool _directed)
        : Graph(_vertex_count, _directed) {
    adj_list = std::vector<std::vector<Graph::Vertex>>(vertex_count);
}

void AdjListGraph::AddEdge(const Graph::Vertex &from, const Graph::Vertex &to) {
    ++edge_count;
    adj_list[from].push_back(to);
    if (!directed) {
        adj_list[to].push_back(from);
    }
}

std::vector<Graph::Vertex> AdjListGraph::GetNeighbours(const Graph::Vertex &v) const {
    return adj_list[v];
}

size_t AdjListGraph::GetOutcomeDegree(const Graph::Vertex &v) const {
    return adj_list[v].size();
}

size_t AdjListGraph::GetIncomeDegree(const Graph::Vertex &v) const {
    size_t degree = 0;
    for (Graph::Vertex u = 0; u < vertex_count; ++u) {
        for (Graph::Vertex i : adj_list[u]) {
            if (i == v) {
                ++degree;
            }
        }
    }
    return degree;
}