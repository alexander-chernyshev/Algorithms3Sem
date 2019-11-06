<<<<<<< Updated upstream
//
// Created by alexander on 19.10.2019.
//

=======
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

std::vector<std::vector<char>> MakeTrackMap(size_t height, size_t width) {
    std::vector<std::vector<char>> map(height);
    for (std::vector<char> &v : map) {
        v.resize(width);
    }
    return map;
}

size_t GetVertexIndex(size_t width, size_t i, size_t j) {
    return i * width + j;
}

std::pair<size_t, size_t> GetCoordinates(size_t width, size_t index) {
    std::pair<size_t, size_t> result;
    result.first = index / width;
    result.second = index % width;
    return result;
}

std::vector<size_t> GetPossibleMoves(const std::vector<std::vector<char>> &map,
                                     size_t map_height,
                                     size_t map_width,
                                     size_t vertex_index) {
    std::pair<size_t, size_t> coordinates = GetCoordinates(map_width, vertex_index);
    int x = coordinates.second;
    int y = coordinates.first;
    int i = 0;
    size_t left_move, right_move, up_move, down_move;
    while (x - i >= 0 && map[y][x - i] != '#') {
        ++i;
    }
    --i;
    left_move = x - i + i / 2;
    i = 0;
    while (x + i < map_width && map[y][x + i] != '#') {
        ++i;
    }
    --i;
    right_move = x + i - i / 2;
    i = 0;
    while (y - i >= 0 && map[y - i][x] != '#') {
        ++i;
    }
    --i;
    up_move = y - i + i / 2;
    i = 0;
    while (y + i < map_height && map[y + i][x] != '#') {
        ++i;
    }
    --i;
    down_move = y + i - i / 2;
    std::vector<size_t> moves;
    moves.push_back(GetVertexIndex(map_width, y, left_move));
    moves.push_back(GetVertexIndex(map_width, y, right_move));
    moves.push_back(GetVertexIndex(map_width, up_move, x));
    moves.push_back(GetVertexIndex(map_width, down_move, x));
    return moves;
}

int main() {
    size_t height, width;
    std::cin >> height >> width;
    std::vector<std::vector<char>> field = MakeTrackMap(height, width);
    for (size_t i = 0; i < height; ++i) {
        for (size_t j = 0; j < width; ++j) {
            std::cin >> field[i][j];
        }
    }
    AdjListGraph graph(height * width, true);
    size_t start_index = 0, finish_index = 0;
    for (size_t i = 0; i < height; ++i) {
        for (size_t j = 0; j < width; ++j) {
            if (field[i][j] != '#') {
                std::vector<size_t> move_indexes = GetPossibleMoves(field, height, width, GetVertexIndex(width, i, j));
                for (size_t index : move_indexes) {
                    graph.AddEdge(GetVertexIndex(width, i, j), index);
                }
            }
            if (field[i][j] == 'S') {
                start_index = GetVertexIndex(width, i, j);
            }
            if (field[i][j] == 'T') {
                finish_index = GetVertexIndex(width, i, j);
            }
        }
    }
    std::vector<Graph::Vertex> shortest_path = GraphProcessing::GetShortestPath(graph, start_index, finish_index);
    if (shortest_path.empty()) {
        std::cout << -1;
    } else {
        std::cout << shortest_path.size() - 1;
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
>>>>>>> Stashed changes
