#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <unordered_set>
#include <set>
#include <map>

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

    virtual bool CheckEdge(const Vertex &from, const Vertex &to) const = 0;

    size_t GetVertexCount() const;
};

class AdjListGraph : public Graph {
private:
    std::vector<std::vector<Vertex>> adj_list;
public:
    AdjListGraph(size_t _vertex_count, bool _directed);

    std::vector<Vertex> GetNeighbours(const Vertex &v) const override;

    void AddEdge(const Vertex &from, const Vertex &to) override;

    bool CheckEdge(const Vertex &from, const Vertex &to) const override;
};

namespace GraphProcessing {
    enum VertexMark {
        WHITE, GREY, BLACK
    };
    const int UNVISITED_VERTEX = -1;

    void DFS(const Graph &g, std::vector<VertexMark> &color, size_t &timer,
             Graph::Vertex v,
             std::vector<size_t> &tin, std::vector<size_t> &fup,
             std::vector<std::pair<Graph::Vertex, Graph::Vertex>> &bridges, Graph::Vertex prev = UNVISITED_VERTEX) {
        color[v] = GREY;
        ++timer;
        tin[v] = timer;
        fup[v] = timer;
        for (Graph::Vertex &u : g.GetNeighbours(v)) {
            if (u != prev) {
                if (color[u] != WHITE) {
                    fup[v] = std::min(fup[v], tin[u]);
                } else {
                    DFS(g, color, timer, u, tin, fup, bridges, v);
                    fup[v] = std::min(fup[v], fup[u]);
                    if (fup[u] > tin[v]) {
                        bridges.emplace_back(u, v);
                    }
                }
            }
        }
        ++timer;
        color[v] = BLACK;
    }

    std::vector<std::pair<Graph::Vertex, Graph::Vertex>> GetBridges(const Graph &graph) {
        std::vector<VertexMark> color(graph.GetVertexCount(), WHITE);
        std::vector<size_t> tin(graph.GetVertexCount());
        std::vector<size_t> fup(graph.GetVertexCount());
        std::vector<std::pair<Graph::Vertex, Graph::Vertex>> bridges;
        size_t timer = 0;
        for (Graph::Vertex i = 0; i < graph.GetVertexCount(); ++i) {
            if (color[i] == WHITE) {
                DFS(graph, color, timer, i, tin, fup, bridges);
            }
        }
        return bridges;
    }
}


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    size_t vertex_count, edge_count;
    std::cin >> vertex_count >> edge_count;
    AdjListGraph graph(vertex_count, false);
    std::map<std::pair<Graph::Vertex, Graph::Vertex>, int> edge_index;
    for (size_t i = 0; i < edge_count; ++i) {
        size_t from, to;
        std::cin >> from >> to;
        if (edge_index[{from - 1, to - 1}] == 0 && edge_index[{to - 1, from - 1}] == 0) {
            edge_index[{from - 1, to - 1}] = i + 1;
            edge_index[{to - 1, from - 1}] = i + 1;
            graph.AddEdge(from - 1, to - 1);
        } else {
            edge_index[{from - 1, to - 1}] = -1;
            edge_index[{to - 1, from - 1}] = -1;
        }
    }
    std::vector<std::pair<Graph::Vertex, Graph::Vertex>> bridges = GraphProcessing::GetBridges(graph);
    std::set<size_t> bridge_indexes;
    for (std::pair<Graph::Vertex, Graph::Vertex> e : bridges) {
        if (edge_index[e] != -1) {
            bridge_indexes.insert(edge_index[e]);
        }
    }
    std::cout << bridge_indexes.size() << '\n';
    for (size_t i : bridge_indexes) {
        std::cout << i << '\n';
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

bool AdjListGraph::CheckEdge(const Graph::Vertex &from, const Graph::Vertex &to) const {
    for (Graph::Vertex v : adj_list[from]) {
        if (v == to) {
            return true;
        }
    }
    return false;
}