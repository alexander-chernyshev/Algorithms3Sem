#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <unordered_map>

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

    size_t GetEdgeCount() const;
};

class AdjListGraph : public Graph {
private:
    std::vector<std::vector<Vertex>> adj_list;
public:
    AdjListGraph(size_t _vertex_count, bool _directed);

    AdjListGraph(size_t _vertex_count, const std::vector<std::vector<size_t >> &adj_matrix, bool _directed);

    std::vector<Vertex> GetNeighbours(const Vertex &v) const override;

    void AddEdge(const Vertex &from, const Vertex &to) override;
};

class MatrixGraph : public Graph {
private:
    std::vector<std::vector<size_t>> has_edge;
public:
    MatrixGraph(size_t _vertex_count, bool _directed);

    MatrixGraph(size_t _vertex_count, const std::vector<std::vector<size_t >> &adj_matrix, bool _directed);

    std::vector<Vertex> GetNeighbours(const Vertex &v) const override;

    void AddEdge(const Vertex &from, const Vertex &to) override;
};

const size_t GRAPH_SIZE = 10000;
const Graph::Vertex FIRST_VERTEX = 1111;

size_t LeftShift(size_t value) {
    size_t res = (value % 1000) * 10;
    res += value / 1000;
    return res;
}

size_t RightShift(size_t value) {
    size_t res = value / 10;
    res += (value % 10) * 1000;
    return res;
}

bool HasZero(size_t value) {
    while (value != 0) {
        if (value % 10 == 0) {
            return true;
        }
        value /= 10;
    }
    return false;
}

namespace GraphProcessing {

    void bfs(const Graph &g, Graph::Vertex start, Graph::Vertex finish,
             std::unordered_map<Graph::Vertex, Graph::Vertex> &prev) {
        std::queue<Graph::Vertex> q;
        q.push(start);
        while (!q.empty()) {
            Graph::Vertex current_vertex = q.front();
            q.pop();
            if (current_vertex == finish) {
                return;
            }
            for (Graph::Vertex v : g.GetNeighbours(current_vertex)) {
                if (prev.find(v) == prev.end()) {
                    q.push(v);
                    prev[v] = current_vertex;
                }
            }
        }
    }

    std::vector<Graph::Vertex> GetShortestPath(const Graph &g, Graph::Vertex from, Graph::Vertex to) {
        if (from == to) {
            return std::vector<Graph::Vertex>(1, from);
        }
        std::unordered_map<Graph::Vertex, Graph::Vertex> prev;
        bfs(g, from, to, prev);
        if (prev.find(to) == prev.end()) {
            return std::vector<Graph::Vertex>();
        }
        std::vector<Graph::Vertex> path;
        Graph::Vertex current = to;
        while (current != from) {
            path.push_back(current);
            current = prev[current];
        }
        path.push_back(from);
        std::reverse(path.begin(), path.end());
        return path;
    }
}

int main() {
    AdjListGraph graph(GRAPH_SIZE, true);
    for (Graph::Vertex v = FIRST_VERTEX; v < GRAPH_SIZE; ++v) {
        if (!HasZero(v)) {
            graph.AddEdge(v, LeftShift(v));
            graph.AddEdge(v, RightShift(v));
            if (v / 1000 < 9) {
                graph.AddEdge(v, v + 1000);
            }
            if (v % 10 > 1) {
                graph.AddEdge(v, v - 1);
            }
        }
    }
    Graph::Vertex from, to;
    std::cin >> from >> to;
    std::vector<Graph::Vertex> path = GraphProcessing::GetShortestPath(graph, from, to);
    for (Graph::Vertex v : path) {
        std::cout << v << '\n';
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

AdjListGraph::AdjListGraph(size_t _vertex_count, const std::vector<std::vector<size_t>> &adj_matrix, bool _directed)
        : Graph(_vertex_count, _directed) {
    adj_list = std::vector<std::vector<Graph::Vertex>>(vertex_count);
    for (size_t i = 0; i < vertex_count; ++i) {
        for (size_t j = 0; j < vertex_count; ++j) {
            if (adj_matrix[i][j] != 0) {
                AddEdge(i, j);
            }
        }
    }
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

MatrixGraph::MatrixGraph(size_t _vertex_count, const std::vector<std::vector<size_t >> &adj_matrix, bool _directed)
        : Graph(_vertex_count, _directed) {
    has_edge = adj_matrix;
    for (size_t i = 0; i < vertex_count; ++i) {
        for (size_t j = 0; j < vertex_count; ++j) {
            if (has_edge[i][j] != 0) {
                ++edge_count;
            }
        }
    }
    if (!directed) {
        edge_count /= 2;
    }
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