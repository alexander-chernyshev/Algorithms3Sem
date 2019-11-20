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
    std::map<std::pair<Graph::Vertex, Graph::Vertex>, int> weight;
public:
    WeightedAdjListGraph(size_t _vertex_count, bool _directed) : Graph(_vertex_count, _directed),
                                                                 AdjListGraph(_vertex_count, _directed),
                                                                 WeightedGraph(_vertex_count, _directed) {};

    void AddEdge(const Vertex &from, const Vertex &to, int edge_weight) override;

    int GetEdgeWeight(const Vertex &from, const Vertex &to) const override;

    void SetEdgeWeight(const Graph::Vertex &from, const Graph::Vertex &to, int edge_weight) override;
};

namespace GraphProcessing {
    const Graph::Vertex NO_ANCESTOR = -1;

    std::vector<Graph::Vertex> SelectCycle(Graph::Vertex v, const std::vector<Graph::Vertex> &prev) {
        if (prev[v] == -1) {
            return std::vector<Graph::Vertex>();
        }
        std::vector<Graph::Vertex> cycle;
        cycle.push_back(v);
        Graph::Vertex current = prev[v];
        while (current != v && prev[current] != NO_ANCESTOR) {
            cycle.push_back(current);
            current = prev[current];
        }
        if (prev[current] == NO_ANCESTOR) {
            return std::vector<Graph::Vertex>();
        }
        std::reverse(cycle.begin(), cycle.end());
        return cycle;
    }

    void FordBellman(const WeightedGraph &graph, Graph::Vertex start,
                     std::vector<int> &dist, std::vector<Graph::Vertex> &prev) {
        dist[start] = 0;
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
    }

    Graph::Vertex GetVertexOnNegativeCycle(const WeightedGraph &graph,
                                           const std::vector<int> &dist, const std::vector<Graph::Vertex> &prev) {
        for (Graph::Vertex u = 0; u < graph.GetVertexCount(); ++u) {
            for (Graph::Vertex v : graph.GetNeighbours(u)) {
                if (dist[v] > dist[u] + graph.GetEdgeWeight(u, v)) {
                    Graph::Vertex cycle_vertex = v;
                    for (size_t i = 0; i < graph.GetVertexCount(); ++i) {
                        cycle_vertex = prev[cycle_vertex];
                    }
                    return cycle_vertex;
                }
            }
        }
        return -1;
    }

    std::vector<Graph::Vertex> GetNegativeCycle(const WeightedGraph &graph, Graph::Vertex start) {
        std::vector<int> dist(graph.GetVertexCount(), MAX_WEIGHT);
        std::vector<Graph::Vertex> prev(graph.GetVertexCount(), NO_ANCESTOR);
        FordBellman(graph, start, dist, prev);
        Graph::Vertex cycle_vertex = GetVertexOnNegativeCycle(graph, dist, prev);
        if (cycle_vertex == -1) {
            return std::vector<Graph::Vertex>();
        } else {
            return SelectCycle(cycle_vertex, prev);
        }
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
        neg_cycle = GraphProcessing::GetNegativeCycle(graph, i);
        if (!neg_cycle.empty()) {
            neg_cycle.push_back(neg_cycle[0]);
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
    weight[{from, to}] = edge_weight;
    if (!directed) {
        weight[{to, from}] = edge_weight;
    }
}

int WeightedAdjListGraph::GetEdgeWeight(const Graph::Vertex &from, const Graph::Vertex &to) const {
    return weight.at(std::make_pair(from, to));
}

void WeightedAdjListGraph::SetEdgeWeight(const Graph::Vertex &from, const Graph::Vertex &to, int edge_weight) {
    weight[{from, to}] = edge_weight;
    if (!directed) {
        weight[{to, from}] = edge_weight;
    }
}