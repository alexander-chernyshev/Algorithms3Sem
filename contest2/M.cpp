#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <set>
#include <algorithm>
#include <utility>

const int MAX_FLOW = 1000000;

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
    std::map<std::pair<Graph::Vertex, Graph::Vertex>, size_t> weight;
public:
    WeightedAdjListGraph(size_t _vertex_count, bool _directed) : Graph(_vertex_count, _directed),
                                                                 AdjListGraph(_vertex_count, _directed),
                                                                 WeightedGraph(_vertex_count, _directed) {};

    void AddEdge(const Vertex &from, const Vertex &to, int edge_weight) override;

    int GetEdgeWeight(const Vertex &from, const Vertex &to) const override;

    void SetEdgeWeight(const Graph::Vertex &from, const Graph::Vertex &to, int edge_weight) override;
};

namespace GraphProcessing {
    int max_flow_search(const WeightedGraph &graph, Graph::Vertex from, Graph::Vertex to, std::vector<bool> &visited,
                        std::map<std::pair<Graph::Vertex, Graph::Vertex>, int> &residual_capacity, int current_flow = MAX_FLOW) {
        if (from == to) {
            return current_flow;
        }
        visited[from] = true;
        for (Graph::Vertex v = 0; v < graph.GetVertexCount(); ++v) {
            if (!visited[v] && residual_capacity[{from, v}] > 0) {
                int delta = max_flow_search(graph, v, to, visited, residual_capacity, std::min(current_flow, residual_capacity[{from, v}]));
                if (delta > 0) {
                    residual_capacity[{from, v}] -= delta;
                    residual_capacity[{v, from}] += delta;
                    return delta;
                }
            }
        }
        return 0;
    }

    int FordFulkersonMaxFlow(const WeightedGraph &g, Graph::Vertex start, Graph::Vertex finish) {
        std::map<std::pair<Graph::Vertex, Graph::Vertex>, int> residual_network;
        for (Graph::Vertex u = 0; u < g.GetVertexCount(); ++u) {
            for (auto v : g.GetNeighbours(u)) {
                residual_network[{u, v}] = g.GetEdgeWeight(u, v);
            }
        }
        int delta = MAX_FLOW;
        int graph_flow = 0;
        while (delta > 0) {
            std::vector<bool> visited(g.GetVertexCount());
            delta = max_flow_search(g, start, finish, visited, residual_network);
            graph_flow += delta;
        }
        return graph_flow;
    }
}

int main() {
    size_t vertex_count, edge_count;
    std::cin >> vertex_count >> edge_count;
    WeightedAdjListGraph graph(vertex_count, true);
    for (size_t i = 0; i < edge_count; ++i) {
        size_t from, to;
        int weight;
        std::cin >> from >> to >> weight;
        graph.AddEdge(from - 1, to - 1, weight);
    }
    std::cout << GraphProcessing::FordFulkersonMaxFlow(graph, 0, vertex_count - 1);
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
    weight[{from, to}] += edge_weight;
    if (!directed) {
        weight[{to, from}] += edge_weight;
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