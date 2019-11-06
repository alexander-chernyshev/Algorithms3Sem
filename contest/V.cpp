#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <utility>
#include <map>

const int MAX_WEIGHT = 1000000000;

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
        return a.first < b.first;
    }
};

class WeightInterface {
protected:
    std::map<std::pair<Graph::Vertex, Graph::Vertex>, std::vector<std::pair<int, int>>> weight;
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

    void AddEdge(const Vertex &from, const Vertex &to, int first_val, int second_val);

    std::vector<std::pair<int, int>> GetEdgeWeight(const Vertex &from, const Vertex &to) const;
};

namespace GraphProcessing {
    const int ROOT_VERTEX = -1;


    std::vector<int> FordBellman(const WeightedAdjListGraph &graph, Graph::Vertex start) {
        std::vector<int> dist(graph.GetVertexCount(), MAX_WEIGHT);
        dist[start] = 0;
        for (size_t i = 0; i < graph.GetVertexCount(); ++i) {
            for (Graph::Vertex u = 0; u < graph.GetVertexCount(); ++u) {
                for (Graph::Vertex v : graph.GetNeighbours(u)) {
                    for (std::pair<int, int> e: graph.GetEdgeWeight(u, v)) {
                        if (e.first >= dist[u] && e.second < dist[v]) {
                            dist[v] = e.second;
                        }
                    }
                }
            }
        }
        return dist;
    }
}

int main() {
    size_t vertex_count, edge_count;
    size_t start, finish;
    std::cin >> vertex_count;
    std::cin >> start >> finish;
    std::cin >> edge_count;
    WeightedAdjListGraph graph(vertex_count, true);
    for (size_t i = 0; i < edge_count; ++i) {
        size_t from, to;
        int dep_time, arr_time;
        std::cin >> from >> dep_time >> to >> arr_time;
        graph.AddEdge(from - 1, to - 1, dep_time, arr_time);
    }
    std::vector<int> distances = GraphProcessing::FordBellman(graph, start - 1);
    int final_time = distances[finish - 1];
    std::cout << final_time;
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
        AdjListGraph(_vertex_count, _directed), WeightInterface() {};

void WeightedAdjListGraph::AddEdge(const Graph::Vertex &from, const Graph::Vertex &to, int first_val, int second_val) {
    AdjListGraph::AddEdge(from, to);
    weight[{from, to}].push_back({first_val, second_val});
}

std::vector<std::pair<int, int>>
WeightedAdjListGraph::GetEdgeWeight(const Graph::Vertex &from, const Graph::Vertex &to) const {
    return weight.at({from, to});
}
