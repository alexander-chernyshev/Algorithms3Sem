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
    std::vector<size_t> GetDistances(const Graph& g, const std::vector<Graph::Vertex>& start_points) {
        std::vector<size_t> distances(g.GetVertexCount(), g.GetVertexCount()+1);
        for (auto v: start_points) {
            distances[v] = 0;
            std::vector<bool> visited(g.GetVertexCount());
            std::queue<Graph::Vertex> q;
            q.push(v);
            visited[v] = true;
            while (!q.empty()) {
                Graph::Vertex current_vertex = q.front();
                q.pop();
                for (auto u : g.GetNeighbours(current_vertex)) {
                    if (!visited[u]) {
                        q.push(u);
                        visited[u] = true;
                        if (distances[u] > distances[current_vertex] + 1)
                            distances[u] = distances[current_vertex] + 1;
                    }
                }
            }
        }
        return distances;
    }

}

int main() {
    size_t height, width;
    std::cin >> height >> width;
    AdjListGraph g(width*height, false);
    for (size_t i = 0; i < height; ++i) {
        for (size_t j = 1; j < width; ++j) {
            g.AddEdge(i*width+j, i*width+j-1);
        }
    }
    for (size_t i = 1; i < height; ++i) {
        for (size_t j = 0; j < width; ++j) {
            g.AddEdge(i*width+j, (i-1)*width+j);
        }
    }
    std::vector<Graph::Vertex> one_points;
    for (size_t i = 0; i < height; ++i) {
        for (size_t j = 0; j < width; ++j) {
            size_t val;
            std::cin >> val;
            if (val == 1) {
                one_points.push_back(i*width+j);
            }
        }
    }
    std::vector<size_t> distances = GraphProcessing::GetDistances(g, one_points);
    for (size_t i = 0; i < height; ++i) {
        for (size_t j = 0; j < width; ++j) {
            std::cout << distances[i*width+j] << ' ';
        }
        std::cout << '\n';
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
