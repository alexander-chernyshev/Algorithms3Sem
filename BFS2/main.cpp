#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

class Graph {
protected:
    size_t vertex_count;
    size_t edge_count = 0;
    bool directed;
public:
    typedef size_t Vertex;

    Graph(size_t _vertex_count, bool _directed);

    size_t GetVertexCount() const;

    virtual std::vector<Vertex> GetNeighbours(const Vertex &v) const = 0;

    virtual void AddEdge(const Vertex &from, const Vertex &to) = 0;
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
    const int UNVISITED_VERTEX = -1;

    void bfs(const Graph &g, const std::vector<Graph::Vertex> &start_vertices, std::vector<size_t> &distance) {
        const size_t default_distance = g.GetVertexCount() + 1;
        std::queue<Graph::Vertex> q;
        distance = std::vector<size_t>(g.GetVertexCount(), default_distance);
        for (const Graph::Vertex &v : start_vertices) {
            distance[v] = 0;
            q.push(v);
        }
        while (!q.empty()) {
            Graph::Vertex current_vertex = q.front();
            q.pop();
            for (auto v : g.GetNeighbours(current_vertex)) {
                if (distance[v] > distance[current_vertex] + 1) {
                    q.push(v);
                    distance[v] = distance[current_vertex] + 1;
                }
            }
        }
    }

    std::vector<size_t> GetDistances(const Graph &g, const std::vector<Graph::Vertex> &start_points) {
        std::vector<size_t> distance;
        bfs(g, start_points, distance);
        return distance;
    }

    size_t GetVertexIndex(size_t graph_width, size_t i, size_t j) {
        return i * graph_width + j;
    };

    AdjListGraph GraphGeneration(size_t width, size_t height) {
        AdjListGraph graph(width * height, false);
        for (size_t i = 0; i < height; ++i) {
            for (size_t j = 1; j < width; ++j) {
                graph.AddEdge(GetVertexIndex(width, i, j), GetVertexIndex(width, i, j - 1));
            }
        }
        for (size_t i = 1; i < height; ++i) {
            for (size_t j = 0; j < width; ++j) {
                graph.AddEdge(GetVertexIndex(width, i, j), GetVertexIndex(width, i - 1, j));
            }
        }
        return graph;
    }
}


int main() {
    size_t height, width;
    std::cin >> height >> width;
    AdjListGraph g = GraphProcessing::GraphGeneration(width, height);
    std::vector<Graph::Vertex> one_points;
    for (size_t i = 0; i < height; ++i) {
        for (size_t j = 0; j < width; ++j) {
            size_t val;
            std::cin >> val;
            if (val == 1) {
                one_points.push_back(GraphProcessing::GetVertexIndex(width, i, j));
            }
        }
    }
    std::vector<size_t> distances = GraphProcessing::GetDistances(g, one_points);
    for (size_t i = 0; i < height; ++i) {
        for (size_t j = 0; j < width; ++j) {
            std::cout << distances[GraphProcessing::GetVertexIndex(width, i, j)] << ' ';
        }
        std::cout << '\n';
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
