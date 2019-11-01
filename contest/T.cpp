#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <set>
#include <algorithm>
#include <utility>

const int MAX_WEIGHT = 100000;

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

struct pair_cmp {
    bool operator()(std::pair<Graph::Vertex, Graph::Vertex> a, std::pair<Graph::Vertex, Graph::Vertex> b) {
        return a.second > b.second;
    }
};

class WeightInterface {
protected:
    std::map<std::pair<Graph::Vertex, Graph::Vertex>, int> weight;
public:
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

    int GetEdgeWeight(Vertex from, Vertex to) const;

    void SetEdgeWeight(const Graph::Vertex &from, const Graph::Vertex &to, int edge_weight);
};

namespace GraphProcessing {
    const int ROOT_VERTEX = -1;

    std::vector<Graph::Vertex> FindNegativeCycle(const WeightedAdjListGraph &graph, Graph::Vertex start) {
        std::vector<int> dist(graph.GetVertexCount(), MAX_WEIGHT);
        dist[start] = 0;
        std::vector<int> prev(graph.GetVertexCount(), ROOT_VERTEX);
        for (size_t i = 0; i < graph.GetVertexCount(); ++i) {
            for (Graph::Vertex u = 0; u < graph.GetVertexCount(); ++u) {
                for (Graph::Vertex v : graph.GetNeighbours(u)) {
                    if (dist[v] > dist[u] + graph.GetEdgeWeight(u, v)) {
                        dist[v] = dist[u] + graph.GetEdgeWeight(u, v);
                        prev[v] = u;
                    }
                }
            }
        }
        for (Graph::Vertex u = 0; u < graph.GetVertexCount(); ++u) {
            for (Graph::Vertex v : graph.GetNeighbours(u)) {
                if (dist[v] > dist[u] + graph.GetEdgeWeight(u, v)) {
                    std::vector<Graph::Vertex> cycle;
                    Graph::Vertex cycle_vertex = v;
                    for (size_t i = 0; i < graph.GetVertexCount(); ++i) {
                        cycle_vertex = prev[cycle_vertex];
                    }
                    Graph::Vertex current = cycle_vertex;
                    while (prev[current] != cycle_vertex) {
                        cycle.push_back(current);
                        current = prev[current];
                    }
                    cycle.push_back(current);
                    cycle.push_back(prev[current]);
                    std::reverse(cycle.begin(), cycle.end());
                    return cycle;
                }
            }
        }
        return std::vector<Graph::Vertex>();
    }
}

int main() {
    size_t vertex_count;
    std::cin >> vertex_count;
    WeightedAdjListGraph graph(vertex_count, true);
    for (size_t i = 0; i < vertex_count; ++i) {
        for (size_t j = 0; j < vertex_count; ++j) {
            int value;
            std::cin >> value;
            if (value != MAX_WEIGHT) {
                graph.AddEdge(i, j, value);
            }
        }
    }
    std::vector<Graph::Vertex> neg_cycle;
    for (Graph::Vertex i = 0; i < vertex_count; ++i) {
        neg_cycle = GraphProcessing::FindNegativeCycle(graph, i);
        if (!neg_cycle.empty()) {
            std::cout << "YES\n" << neg_cycle.size() << '\n';
            for (Graph::Vertex v : neg_cycle) {
                std::cout << v + 1 << ' ';
            }
            break;
        }
    }
    if (neg_cycle.empty()) {
        std::cout << "NO\n";
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
    adj_list.resize(_vertex_count);
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
        AdjListGraph(_vertex_count, _directed), WeightInterface() {};

void WeightedAdjListGraph::AddEdge(const Graph::Vertex &from, const Graph::Vertex &to, int edge_weight) {
    AdjListGraph::AddEdge(from, to);
    weight[{from, to}] = edge_weight;
    if (!directed) {
        weight[{to, from}] = edge_weight;
    }
}

int WeightedAdjListGraph::GetEdgeWeight(Graph::Vertex from, Graph::Vertex to) const {
    return weight.at(std::make_pair(from, to));
}

void WeightedAdjListGraph::SetEdgeWeight(const Graph::Vertex &from, const Graph::Vertex &to, int edge_weight) {
    weight[{from, to}] = edge_weight;
    if (!directed) {
        weight[{to, from}] = edge_weight;
    }
}