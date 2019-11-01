#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <utility>
#include <algorithm>

const int MAX_WEIGHT = 1000000000;

class WeightInterface {
protected:
    std::vector<std::vector<int>> weight;
public:
    explicit WeightInterface(size_t _vertex_count) {
        weight.resize(_vertex_count);
        for (size_t i = 0; i < _vertex_count; ++i) {
            weight[i].assign(_vertex_count, MAX_WEIGHT);
        }
    }
};

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

    bool IsDirected() const;
};

class AdjListGraph : public Graph {
private:
    std::vector<std::vector<Vertex>> adj_list;
public:
    AdjListGraph(size_t _vertex_count, bool _directed);

    std::vector<Vertex> GetNeighbours(const Vertex &v) const override;

    void AddEdge(const Vertex &from, const Vertex &to) override;
};

class WeightedAdjListGraph : public AdjListGraph, WeightInterface {
public:
    WeightedAdjListGraph(size_t _vertex_count, bool _directed);

    void AddEdge(const Vertex &from, const Vertex &to, int edge_weight);

    int GetEdgeWeight(const Vertex &from, const Vertex &to) const;

    void SetEdgeWeight(const Graph::Vertex &from, const Graph::Vertex &to, int edge_weight);
};

namespace GraphProcessing {
    const int ROOT_VERTEX = -1;

    enum VertexMark {
        WHITE, GREY, BLACK
    };

    void GetVerticesInSameComponent(const Graph &g, std::vector<VertexMark> &color, Graph::Vertex v,
                                    std::set<Graph::Vertex> &component) {
        color[v] = GREY;
        component.insert(v);
        for (Graph::Vertex &u : g.GetNeighbours(v)) {
            if (color[u] == WHITE) {
                GetVerticesInSameComponent(g, color, u, component);
            }
        }
        color[v] = BLACK;
    }

    std::set<Graph::Vertex> GetComponent(const Graph &g, Graph::Vertex v) {
        size_t vertex_count = g.GetVertexCount();
        std::set<Graph::Vertex> component;
        std::vector<VertexMark> color(vertex_count, WHITE);
        GetVerticesInSameComponent(g, color, v, component);
        return component;
    }


    std::vector<int> FordBellman(const WeightedAdjListGraph &graph, Graph::Vertex start, size_t max_len) {
        std::vector<std::vector<int>> dist(max_len + 1);
        for (size_t i = 0; i < max_len + 1; ++i) {
            dist[i].assign(graph.GetVertexCount(), MAX_WEIGHT);
        }
        dist[0][start] = 0;
        for (size_t i = 0; i < max_len; ++i) {
            for (Graph::Vertex u = 0; u < graph.GetVertexCount(); ++u) {
                for (Graph::Vertex v : graph.GetNeighbours(u)) {
                    dist[i + 1][v] = std::min(dist[i + 1][v], dist[i][u] + graph.GetEdgeWeight(u, v));
                }
            }
        }
        return dist[max_len];
    }
}

int main() {
    size_t vertex_count, edge_count, max_len, start, finish;
    std::cin >> vertex_count >> edge_count >> max_len >> start >> finish;
    WeightedAdjListGraph graph(vertex_count, true);
    for (size_t i = 0; i < edge_count; ++i) {
        size_t from, to;
        int edge_w;
        std::cin >> from >> to >> edge_w;
        graph.AddEdge(from - 1, to - 1, edge_w);
    }

    for (Graph::Vertex i = 0; i < vertex_count; ++i) {
        graph.AddEdge(i, i, 0);
    }

    std::vector<int> distances = GraphProcessing::FordBellman(graph, start - 1, max_len);
    int distance = distances[finish - 1];
    if (distance == MAX_WEIGHT) {
        std::cout << -1;
    } else {
        std::cout << distance;
    }
    return 0;
}

Graph::Graph(size_t _vertex_count, bool _directed) {
    vertex_count = _vertex_count;
    directed = _directed;
}

bool Graph::IsDirected() const {
    return directed;
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

WeightedAdjListGraph::WeightedAdjListGraph(size_t _vertex_count, bool _directed) :
        AdjListGraph(_vertex_count, _directed), WeightInterface(_vertex_count) {};

void WeightedAdjListGraph::AddEdge(const Graph::Vertex &from, const Graph::Vertex &to, int edge_weight) {
    AdjListGraph::AddEdge(from, to);
    if (edge_weight < weight[from][to]) {
        weight[from][to] = edge_weight;
    }
}

int WeightedAdjListGraph::GetEdgeWeight(const Graph::Vertex &from, const Graph::Vertex &to) const {
    return weight[from][to];
}

void WeightedAdjListGraph::SetEdgeWeight(const Graph::Vertex &from, const Graph::Vertex &to, int edge_weight) {
    weight[from][to] = edge_weight;
}
