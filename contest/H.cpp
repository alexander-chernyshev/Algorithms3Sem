#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <utility>

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

struct ChessTable {
    size_t table_size;

    explicit ChessTable(size_t size) : table_size(size) {};

    size_t GetVertexIndex(size_t i, size_t j) {
        return i * table_size + j;
    }

    std::pair<size_t, size_t> GetCoordinates(Graph::Vertex v) {
        std::pair<size_t, size_t> coordinates;
        coordinates.first = v / table_size;
        coordinates.second = v % table_size;
        return coordinates;
    }

    bool IsOnTable(std::pair<int, int> coordinates) {
        int i = coordinates.first;
        int j = coordinates.second;
        return (i >= 0 && j >= 0 && i < table_size && j < table_size);
    }

    std::vector<Graph::Vertex> GetPossibleMoves(Graph::Vertex v) {
        std::vector<std::pair<int, int>> moves = {{-2, -1},
                                                        {-2, 1},
                                                        {-1, 2},
                                                        {1, 2},
                                                        {2, 1},
                                                        {2, -1},
                                                        {1, -2},
                                                        {-1, -2}};
        std::pair<int, int>
        coordinates = GetCoordinates(v);
        std::vector<Graph::Vertex> possible_vertices;
        for (auto shift : moves) {
            std::pair<int, int> move = {coordinates.first + shift.first, coordinates.second + shift.second};
            if (IsOnTable(move)) {
                possible_vertices.push_back(GetVertexIndex(move.first, move.second));
            }
        }
        return possible_vertices;
    }
};

namespace GraphProcessing {
    const int UNVISITED_VERTEX = -1;

    void bfs(const Graph &g, Graph::Vertex start, std::vector<int> &prev) {
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

    std::vector<Graph::Vertex> GetShortestPath(const Graph &g, Graph::Vertex from, Graph::Vertex to) {
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
    size_t table_size;
    std::cin >> table_size;
    size_t vertex_count = table_size * table_size;
    AdjListGraph graph(vertex_count, false);
    ChessTable table(table_size);
    for (size_t i = 0; i < table_size; ++i) {
        for (size_t j = 0; j < table_size; ++j) {
            Graph::Vertex v = table.GetVertexIndex(i, j);
            std::vector<Graph::Vertex> possible_moves = table.GetPossibleMoves(v);
            for (Graph::Vertex u : possible_moves) {
                graph.AddEdge(v, u);
            }
        }
    }
    size_t from_x, from_y, to_x, to_y;
    std::cin >> from_x >> from_y >> to_x >> to_y;
    Graph::Vertex from = table.GetVertexIndex(from_y - 1, from_x - 1);
    Graph::Vertex to = table.GetVertexIndex(to_y - 1, to_x - 1);
    std::vector<Graph::Vertex> path = GraphProcessing::GetShortestPath(graph, from, to);
    std::cout << path.size() - 1 << '\n';
    for (Graph::Vertex v : path) {
        std::pair<size_t, size_t> coordinates = table.GetCoordinates(v);
        std::cout << coordinates.second + 1 << ' ' << coordinates.first + 1 << '\n';
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