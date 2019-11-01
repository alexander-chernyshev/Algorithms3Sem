#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

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
    enum VertexMark {
        WHITE, GREY, BLACK
    };
    const int UNVISITED_VERTEX = -1;
    size_t timer = 0;

    void DFS(const Graph &g, std::vector<VertexMark> &color, Graph::Vertex v,
             std::vector<size_t> &tin, std::vector<size_t> &fup,
             std::set<Graph::Vertex> &a_points, Graph::Vertex prev = UNVISITED_VERTEX) {
        color[v] = GREY;
        ++timer;
        tin[v] = timer;
        fup[v] = timer;
        size_t children_count = 0;
        for (Graph::Vertex &u : g.GetNeighbours(v)) {
            if (u != prev) {
                if (color[u] != WHITE) {
                    if (fup[v] > tin[u]) {
                        fup[v] = tin[u];
                    }
                } else {
                    DFS(g, color, u, tin, fup, a_points, v);
                    ++children_count;
                    if (fup[v] > fup[u]) {
                        fup[v] = fup[u];
                    }
                    if (fup[u] >= tin[v] && prev != -1) {
                        a_points.insert(v);
                    }
                }
            }
        }
        if (prev == -1 && children_count > 1)
            a_points.insert(v);
        color[v] = BLACK;
    }

    std::set<Graph::Vertex> GetArticulationPoints(const Graph &graph) {
        std::vector<VertexMark> color(graph.GetVertexCount(), WHITE);
        std::vector<size_t> tin(graph.GetVertexCount());
        std::vector<size_t> fup(graph.GetVertexCount());
        std::set<Graph::Vertex> a_points;
        timer = 0;
        for (Graph::Vertex i = 0; i < graph.GetVertexCount(); ++i) {
            if (color[i] == WHITE) {
                DFS(graph, color, i, tin, fup, a_points);
            }
        }
        return a_points;
    }
}


int main() {
    size_t vert_count, edge_count;
    std::cin >> vert_count >> edge_count;
    AdjListGraph graph(vert_count, false);
    for (size_t i = 0; i < edge_count; ++i) {
        size_t from, to;
        std::cin >> from >> to;
        graph.AddEdge(from - 1, to - 1);
    }
    std::set<Graph::Vertex> a_points = GraphProcessing::GetArticulationPoints(graph);
    std::cout << a_points.size() << '\n';
    for (Graph::Vertex v : a_points) {
        std::cout << v + 1 << '\n';
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
