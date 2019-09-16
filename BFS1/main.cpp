#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

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
    void ReadMatrixGraph(Graph& g) {
        size_t vertex_count = g.GetVertexCount();
        for (size_t i = 0; i < vertex_count; ++i) {
            for (size_t j = 0; j < vertex_count; ++j) {
                size_t value;
                std::cin >> value;
                if (value != 0)
                    g.AddEdge(i, j);
            }
        }
    }

    std::vector<Graph::Vertex> GetShortestPath(const Graph& g, Graph::Vertex from, Graph::Vertex to) {
        if (from == to) {
            return std::vector<Graph::Vertex>(1, from);
        }
        std::queue<Graph::Vertex> q;
        std::vector<int> path(g.GetVertexCount(), -1); //использую int вместо size_t чтобы обозначать непосещенные вершины -1
        q.push(from);
        while (!q.empty()) {
            Graph::Vertex current_vertex = q.front();
            q.pop();
            for (auto v : g.GetNeighbours(current_vertex)) {
                if (path[v] == -1) {
                    q.push(v);
                    path[v] = current_vertex;
                }
                if (v == to) {
                    std::vector<Graph::Vertex> path_vertices;
                    while (v != from) {
                        path_vertices.push_back(v);
                        v = path[v];
                    }
                    path_vertices.push_back(from);
                    std::reverse(path_vertices.begin(), path_vertices.end());
                    return path_vertices;
                }
            }
        }
        return std::vector<Graph::Vertex> ();
    }
}

int main() {
    size_t graph_size;
    std::cin >> graph_size;
    AdjListGraph g(graph_size, false);
    GraphProcessing::ReadMatrixGraph(g);
    Graph::Vertex from, to;
    std::cin >> from >> to;
    std::vector<Graph::Vertex> path = GraphProcessing::GetShortestPath(g, from-1, to-1);
    if (path.empty())
        std::cout << -1;
    else if (path.size() == 1) {
        std::cout << 0;
    }
    else {
        std::cout << path.size()-1 << std::endl;
        for (auto v : path)
            std::cout << v+1 << ' ';
    }
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
