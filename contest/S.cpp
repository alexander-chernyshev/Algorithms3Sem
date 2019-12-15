#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <utility>

const int MAX_WEIGHT = 10000000;

const int NO_PATH = 30000;

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

class WeightedGraph : virtual public Graph {
public:
    WeightedGraph(size_t _vertex_count, bool _directed) : Graph(_vertex_count, _directed) {};

    virtual void AddEdge(const Graph::Vertex &from, const Graph::Vertex &to, int edge_weight) = 0;

    virtual int GetEdgeWeight(const Graph::Vertex &from, const Graph::Vertex &to) const = 0;

    virtual void SetEdgeWeight(const Graph::Vertex &from, const Graph::Vertex &to, int edge_weight) = 0;
};

class AdjListGraph : virtual public Graph {
private:
    std::vector<std::vector<Vertex>> adj_list;
public:
    AdjListGraph(size_t _vertex_count, bool _directed) : Graph(_vertex_count, _directed) {
        adj_list.resize(_vertex_count);
    }

    std::vector<Vertex> GetNeighbours(const Vertex &v) const override;

    void AddEdge(const Vertex &from, const Vertex &to) override;
};

class WeightedAdjListGraph : virtual public AdjListGraph, virtual public WeightedGraph {
private:
    std::vector<std::vector<int>> weight;
public:
    WeightedAdjListGraph(size_t _vertex_count, bool _directed) : Graph(_vertex_count, _directed),
                                                                 AdjListGraph(_vertex_count, _directed),
                                                                 WeightedGraph(_vertex_count, _directed) {
        weight.resize(vertex_count);
        for (size_t i = 0; i < vertex_count; ++i) {
            weight[i].assign(vertex_count, MAX_WEIGHT);
        }
    };

    void AddEdge(const Vertex &from, const Vertex &to, int edge_weight) override;

    int GetEdgeWeight(const Vertex &from, const Vertex &to) const override;

    void SetEdgeWeight(const Graph::Vertex &from, const Graph::Vertex &to, int edge_weight) override;
};

struct Edge {
    Graph::Vertex from;
    Graph::Vertex to;
    int weight;

    Edge(Graph::Vertex _from, Graph::Vertex _to, int _weight) : from(_from), to(_to), weight(_weight) {};
};

namespace GraphProcessing {
    const int ROOT_VERTEX = -1;

    enum VertexMark {
        WHITE, GREY, BLACK
    };

    void GetReachableVertices(const Graph &g, std::vector<VertexMark> &color, Graph::Vertex v,
                              std::vector<Graph::Vertex> &reachable_vertices) {
        color[v] = GREY;
        reachable_vertices.push_back(v);
        for (Graph::Vertex &u : g.GetNeighbours(v)) {
            if (color[u] == WHITE) {
                GetReachableVertices(g, color, u, reachable_vertices);
            }
        }
        color[v] = BLACK;
    }

    std::vector<Graph::Vertex> GetReachableFromVertex(const Graph &g, Graph::Vertex v) {
        std::vector<VertexMark> color(g.GetVertexCount(), WHITE);
        std::vector<Graph::Vertex> reachable_vertices;
        GetReachableVertices(g, color, v, reachable_vertices);
        return reachable_vertices;
    }

    std::vector<int> GetAllDistances(const WeightedAdjListGraph &graph, Graph::Vertex start) {
        std::vector<int> dist(graph.GetVertexCount(), MAX_WEIGHT);
        dist[start] = 0;
        for (size_t i = 0; i < graph.GetVertexCount() - 1; ++i) {
            for (Graph::Vertex u = 0; u < graph.GetVertexCount(); ++u) {
                for (Graph::Vertex v : graph.GetNeighbours(u)) {
                    dist[v] = std::min(dist[v], dist[u] + graph.GetEdgeWeight(u, v));
                }
            }
        }
        std::vector<Graph::Vertex> reachable_vertices = GetReachableFromVertex(graph, start);
        std::vector<int> real_distances(graph.GetVertexCount(), NO_PATH);
        for (auto v : reachable_vertices) {
            real_distances[v] = dist[v];
        }
        return real_distances;
    }
}

int main() {
    size_t vertex_count, edge_count;
    std::cin >> vertex_count >> edge_count;
    WeightedAdjListGraph graph(vertex_count, true);
    for (size_t i = 0; i < edge_count; ++i) {
        size_t from, to;
        int edge_w;
        std::cin >> from >> to >> edge_w;
        graph.AddEdge(from - 1, to - 1, edge_w);
    }
    std::vector<int> distances = GraphProcessing::GetAllDistances(graph, 0);
    for (size_t i = 0; i < vertex_count; ++i) {
        std::cout << distances[i] << ' ';
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

void WeightedAdjListGraph::AddEdge(const Graph::Vertex &from, const Graph::Vertex &to, int edge_weight) {
    AdjListGraph::AddEdge(from, to);
    if (edge_weight < weight[from][to]) {
        weight[from][to] = edge_weight;
        if (!directed) {
            weight[to][from] = edge_weight;
        }
    }
}

int WeightedAdjListGraph::GetEdgeWeight(const Graph::Vertex &from, const Graph::Vertex &to) const {
    return weight[from][to];
}

void WeightedAdjListGraph::SetEdgeWeight(const Graph::Vertex &from, const Graph::Vertex &to, int edge_weight) {
    weight[from][to] = edge_weight;
    if (!directed) {
        weight[from][to] = edge_weight;
    }
}