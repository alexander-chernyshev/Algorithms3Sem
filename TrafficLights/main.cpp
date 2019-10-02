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

    virtual std::vector<Vertex> GetNeighbours(const Vertex& v) const = 0;
    virtual void AddEdge(const Vertex& from, const Vertex& to) = 0;
    virtual size_t GetVertexDegree(const Vertex& v) const = 0;

    size_t GetVertexCount() const;
    size_t GetEdgeCount() const;
};

class AdjListGraph : public Graph{
private:
    std::vector<std::vector<Vertex>> adj_list;
public:
    AdjListGraph(size_t _vertex_count, bool _directed);

    std::vector<Vertex> GetNeighbours(const Vertex& v) const override;
    void AddEdge(const Vertex& from, const Vertex& to) override;
    size_t GetVertexDegree(const Vertex& v) const override;
};

class MatrixGraph : public Graph {
private:
    std::vector<std::vector<size_t>> has_edge;
public:
    MatrixGraph(size_t _vertex_count, bool _directed);

    std::vector<Vertex> GetNeighbours(const Vertex& v) const override;
    void AddEdge(const Vertex& from, const Vertex& to) override;
    size_t GetVertexDegree(const Vertex& v) const override;
};

namespace GraphProcessing {

}

int main() {
    size_t vertex_count, edge_count;
    std::cin >> vertex_count >> edge_count;
    AdjListGraph g(vertex_count, false);
    for (size_t i = 0; i < edge_count; ++i) {
        size_t from, to;
        std::cin >> from >> to;
        g.AddEdge(from - 1, to - 1);
    }
    for (Graph::Vertex v = 0; v < g.GetVertexCount(); ++v) {
        std::cout << g.GetVertexDegree(v) << ' ';
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

size_t Graph::GetEdgeCount() const {
    return edge_count;
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

size_t AdjListGraph::GetVertexDegree(const Graph::Vertex &v) const {
    return adj_list[v].size();
}

MatrixGraph::MatrixGraph(size_t _vertex_count, bool _directed): Graph(_vertex_count, _directed) {
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
    if (has_edge[from][to]) {
        return;
    }
    ++edge_count;
    has_edge[from][to] = 1;
    if (!directed) {
        has_edge[to][from] = 1;
    }
}

size_t MatrixGraph::GetVertexDegree(const Graph::Vertex &v) const {
    size_t degree = 0;
    for (size_t i = 0; i < vertex_count; ++i) {
        if (has_edge[v][i] != 0) {
            ++degree;
        }
    }
    return degree;
}
