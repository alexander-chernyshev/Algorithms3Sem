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

    size_t GetVertexCount() const;
};

class AdjListGraph : public Graph {
private:
    std::vector<std::vector<Vertex>> adj_list;
public:
    AdjListGraph(size_t _vertex_count, bool _directed);

    std::vector<Vertex> GetNeighbours(const Vertex &v) const override;

    void AddEdge(const Vertex &from, const Vertex &to) override;
};
namespace GraphProcessing {
    const int UNVISITED_VERTEX = -1;

    enum VertexMark {
        WHITE, GREY, BLACK
    };

    void DFS(const Graph &g, std::vector<VertexMark> &color, Graph::Vertex v, std::vector<int>& predecessors,
             std::vector<Graph::Vertex> &cycle) {
        color[v] = GREY;
        for (Graph::Vertex &u : g.GetNeighbours(v)) {
            if (color[u] == WHITE) {
                predecessors[u] = v;
                DFS(g, color, u, predecessors, cycle);
            } else if (color[u] == GREY && cycle.empty()) {
                Graph::Vertex current = v;
                while (current != u) {
                    cycle.push_back(current);
                    current = predecessors[current];
                }
                cycle.push_back(u);
                std::reverse(cycle.begin(), cycle.end());
            }
        }
        color[v] = BLACK;
    }

    std::vector<Graph::Vertex> GetCycle(const Graph &g) {
        std::vector<Graph::Vertex> cycle;
        std::vector<VertexMark> color(g.GetVertexCount(), WHITE);
        std::vector<int> predecessors(g.GetVertexCount(), UNVISITED_VERTEX);
        for (Graph::Vertex v = 0; v < g.GetVertexCount(); ++v) {
            if (color[v] == WHITE) {
                DFS(g, color, v, predecessors, cycle);
            }
            if (!cycle.empty()) {
                break;
            }
        }
        return cycle;
    }

}

int main() {
    size_t vertex_count, edge_count;
    std::cin >> vertex_count >> edge_count;
    AdjListGraph g(vertex_count, true);
    for (size_t i = 0; i < edge_count; ++i) {
        size_t from, to;
        std::cin >> from >> to;
        g.AddEdge(from - 1, to - 1);
    }
    std::vector<Graph::Vertex> cycle = GraphProcessing::GetCycle(g);
    if (!cycle.empty()) {
        std::cout << "YES\n";
        for (Graph::Vertex v : cycle) {
            std::cout << v + 1 << ' ';
        }
        std::cout << std::endl;
    } else {
        std::cout << "NO" << std::endl;
    }
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