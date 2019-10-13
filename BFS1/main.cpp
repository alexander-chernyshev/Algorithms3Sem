//Задача А. Путь в графе

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

    virtual std::vector<Vertex> GetNeighbours(const Vertex& v) const = 0;
    virtual void AddEdge(const Vertex& from, const Vertex& to) = 0;

    size_t GetVertexCount() const;
    size_t GetEdgeCount() const;
};

class AdjListGraph : public Graph {
private:
    std::vector<std::vector<Vertex>> adj_list;
public:
    AdjListGraph(size_t _vertex_count, bool _directed);
    AdjListGraph(size_t _vertex_count, const std::vector<std::vector<size_t >>& adj_matrix, bool _directed);

    std::vector<Vertex> GetNeighbours(const Vertex& v) const override;
    void AddEdge(const Vertex& from, const Vertex& to) override;
};

class MatrixGraph : public Graph {
private:
    std::vector<std::vector<size_t>> has_edge;
public:
    MatrixGraph(size_t _vertex_count, bool _directed);
    MatrixGraph(size_t _vertex_count, const std::vector<std::vector<size_t >>& adj_matrix, bool _directed);

    std::vector<Vertex> GetNeighbours(const Vertex& v) const override;
    void AddEdge(const Vertex& from, const Vertex& to) override;
};

namespace GraphProcessing {
    const int UNVISITED_VERTEX = -1;

    void bfs(const Graph& g, Graph::Vertex start, std::vector<int>& prev) {
        std::queue<Graph::Vertex> q;
        prev = std::vector<int>(g.GetVertexCount(), UNVISITED_VERTEX);
        q.push(start);
        while (!q.empty()) {
            Graph::Vertex current_vertex = q.front();
            q.pop();
            for (auto v : g.GetNeighbours(current_vertex)) {
                if (prev[v] == UNVISITED_VERTEX) {
                    q.push(v);
                    prev[v] = current_vertex;
                }
            }
        }
    }

    std::vector<Graph::Vertex> GetShortestPath(const Graph& g, Graph::Vertex from, Graph::Vertex to) {
        if (from == to) {
            return std::vector<Graph::Vertex>(1, from);
        }
        std::vector<int> prev;
        bfs(g, from, prev);
        if (prev[to] == UNVISITED_VERTEX) {
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
    size_t vertex_count;
    std::cin >> vertex_count;
    std::vector<std::vector<size_t >> adj_matrix(vertex_count);
    for (size_t i = 0; i < vertex_count; ++i) {
        adj_matrix[i].resize(vertex_count);
        for (size_t j = 0; j < vertex_count; ++j) {
            std::cin >> adj_matrix[i][j];
        }
    }
    AdjListGraph graph(vertex_count, adj_matrix,false);
    Graph::Vertex from, to;
    std::cin >> from >> to;
    std::vector<Graph::Vertex> path = GraphProcessing::GetShortestPath(graph, from - 1, to - 1);
    if (path.empty()) {
        std::cout << -1;
    } else if (path.size() == 1) {
        std::cout << 0;
    } else {
        std::cout << path.size() - 1 << std::endl;
        for (auto v : path) {
            std::cout << v + 1 << ' ';
        }
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

MatrixGraph::MatrixGraph(size_t _vertex_count, bool _directed): Graph(_vertex_count, _directed) {
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