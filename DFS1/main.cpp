#include <iostream>
#include <vector>

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

class MatrixGraph : public Graph {
private:
    std::vector<std::vector<size_t>> has_edge;
public:
    MatrixGraph(size_t _vertex_count, bool _directed);

    std::vector<Vertex> GetNeighbours(const Vertex &v) const override;

    void AddEdge(const Vertex &from, const Vertex &to) override;
};

namespace GraphProcessing {
    enum VertexMark {
        WHITE, GREY, BLACK
    };

    void GetVerticesInSameComponent(const Graph &g, std::vector<VertexMark> &color, Graph::Vertex v,
                                    std::vector<Graph::Vertex> &component) {
        color[v] = GREY;
        component.push_back(v);
        for (Graph::Vertex &u : g.GetNeighbours(v))
            if (color[u] == WHITE) {
                GetVerticesInSameComponent(g, color, u, component);
            }
        color[v] = BLACK;
    }

    std::vector<Graph::Vertex> GetComponent(const Graph &g, Graph::Vertex v) {
        size_t vertex_count = g.GetVertexCount();
        std::vector<Graph::Vertex> component;
        std::vector<VertexMark> color(vertex_count, WHITE);
        GetVerticesInSameComponent(g, color, v, component);
        return component;
    }
}

int main() {
    size_t count, vert;
    std::cin >> count >> vert;
    AdjListGraph graph(count, false);
    for (size_t i = 0; i < count; ++i) {
        for (size_t j = 0; j < count; ++j) {
            size_t val;
            std::cin >> val;
            if (val != 0)
                graph.AddEdge(i, j);
        }
    }
    std::cout << GraphProcessing::GetComponent(graph, vert - 1).size();
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

MatrixGraph::MatrixGraph(size_t _vertex_count, bool _directed) : Graph(_vertex_count, _directed) {
    has_edge = std::vector<std::vector<size_t>>(vertex_count);
    for (Vertex i = 0; i < vertex_count; ++i)
        has_edge[i].resize(vertex_count);
}

std::vector<Graph::Vertex> MatrixGraph::GetNeighbours(const Graph::Vertex &v) const {
    std::vector<Graph::Vertex> neighbours;
    for (Vertex i = 0; i < vertex_count; ++i) {
        if (has_edge[v][i] != 0)
            neighbours.push_back(i);
    }
    return neighbours;
}

void MatrixGraph::AddEdge(const Graph::Vertex &from, const Graph::Vertex &to) {
    ++edge_count;
    has_edge[from][to] = 1;
    if (!directed) {
        has_edge[to][from] = 1;
    }
}
