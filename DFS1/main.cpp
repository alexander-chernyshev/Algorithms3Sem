#include <iostream>
#include <vector>

class Graph {
protected:
    size_t vertex_count;
    size_t edge_count = 0;
    bool is_oriented;
public:
    typedef size_t Vertex;
    Graph(size_t _vertex_count, bool _is_oriented);

    virtual std::vector<Vertex> GetNeighbours(const Vertex& v) const = 0;
    virtual void AddEdge(const Vertex& from, const Vertex& to) = 0;
    size_t GetVertexCount() const;
};

class AdjListGraph : public Graph{
private:
    std::vector<std::vector<Graph::Vertex>> adj_list;
public:
    AdjListGraph(size_t _vertex_count, bool _is_oriented);

    std::vector<Vertex> GetNeighbours(const Vertex& v) const override;
    void AddEdge(const Vertex& from, const Vertex& to) override;
};

class MatrixGraph : public Graph {
private:
    std::vector<std::vector<size_t>> matrix;
public:
    MatrixGraph(size_t _vertex_count, bool _is_oriented);

    std::vector<Vertex> GetNeighbours(const Vertex& v) const override;
    void AddEdge(const Vertex& from, const Vertex& to) override;
};

namespace GraphProcessing {
    enum VertexMark{WHITE, GREY, BLACK};

    void dfs(const Graph& g, std::vector<size_t>& color, Graph::Vertex v, size_t& counter) {
        color[v] = GREY;
        ++counter;
        for (auto& u : g.GetNeighbours(v))
            if (color[u] == WHITE) {
                dfs(g,color, u, counter);
            }
        color[v] = BLACK;
    }

    int CountVerticesInSameComponent(const Graph& g, Graph::Vertex v) {
        size_t length = g.GetVertexCount();
        size_t counter = 0;
        std::vector<size_t> color(length, WHITE);
        dfs(g, color, v, counter);
        return counter;
    }
}

int main() {
    size_t count, vert;
    std::cin >> count >> vert;
    AdjListGraph g(count, false);
    for (size_t i = 0; i < count; ++i) {
        for (size_t j = 0; j < count; ++j) {
            size_t val;
            std::cin >> val;
            if (val != 0)
                g.AddEdge(i, j);
        }
    }
    std::cout << GraphProcessing::CountVerticesInSameComponent(g, vert-1);
    return 0;
}

Graph::Graph(size_t _vertex_count, bool _is_oriented) {
    vertex_count = _vertex_count;
    is_oriented = _is_oriented;
}

size_t Graph::GetVertexCount() const {
    return vertex_count;
}

AdjListGraph::AdjListGraph(size_t _vertex_count, bool _is_oriented)
        : Graph(_vertex_count, _is_oriented) {
    adj_list = std::vector<std::vector<Graph::Vertex>>(vertex_count);
}

void AdjListGraph::AddEdge(const Graph::Vertex &from, const Graph::Vertex &to) {
    ++edge_count;
    adj_list[from].push_back(to);
    if (!is_oriented)
        adj_list[to].push_back(from);
}

std::vector<Graph::Vertex> AdjListGraph::GetNeighbours(const Graph::Vertex &v) const {
    return adj_list[v];
}

MatrixGraph::MatrixGraph(size_t _vertex_count, bool _is_oriented): Graph(_vertex_count, _is_oriented) {
    matrix = std::vector<std::vector<size_t>>(vertex_count);
    for (size_t i = 0; i < vertex_count; ++i)
        matrix[i].resize(vertex_count);

}

std::vector<Graph::Vertex> MatrixGraph::GetNeighbours(const Graph::Vertex &v) const {
    std::vector<Graph::Vertex> neighbours;
    for (size_t i = 0; i < vertex_count; ++i) {
        if (matrix[v][i] != 0)
            neighbours.push_back(i);
    }
    return neighbours;
}

void MatrixGraph::AddEdge(const Graph::Vertex &from, const Graph::Vertex &to) {
    ++edge_count;
    matrix[from][to] = 1;
}
