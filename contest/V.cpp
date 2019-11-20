#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <utility>
#include <map>

const int MAX_WEIGHT = 1000000000;

struct FlightTime {
    int from;
    int to;

    FlightTime(int _from, int _to): from(_from), to(_to) {}
};

struct flight_time_cmp {
    bool operator()(const FlightTime& a, const FlightTime& b) {
        return a.from < b.from;
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


template <class T>
class WeightedGraph : virtual public Graph {
public:
    WeightedGraph(size_t _vertex_count, bool _directed) : Graph(_vertex_count, _directed) {};

    virtual void AddEdge(const Graph::Vertex &from, const Graph::Vertex &to, T edge_weight) = 0;

    virtual std::vector<T> GetEdgeWeight(const Graph::Vertex &from, const Graph::Vertex &to) const = 0;
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


template <class T>
class WeightedAdjListGraph : virtual public AdjListGraph, virtual public WeightedGraph<T> {
private:
    std::map<std::pair<Graph::Vertex, Graph::Vertex>, std::vector<T>> weight;
public:
    WeightedAdjListGraph(size_t _vertex_count, bool _directed) : Graph(_vertex_count, _directed),
                                                                 AdjListGraph(_vertex_count, _directed),
                                                                 WeightedGraph<T>(_vertex_count, _directed) {};

    void AddEdge(const Vertex &from, const Vertex &to, T edge_weight) override;

    std::vector<T> GetEdgeWeight(const Vertex &from, const Vertex &to) const override;

};

namespace GraphProcessing {
    const int ROOT_VERTEX = -1;

    std::vector<int> FordBellman(const WeightedAdjListGraph<FlightTime> &graph, Graph::Vertex start) {
        std::vector<int> dist(graph.GetVertexCount(), MAX_WEIGHT);
        dist[start] = 0;
        for (size_t i = 0; i < graph.GetVertexCount(); ++i) {
            for (Graph::Vertex u = 0; u < graph.GetVertexCount(); ++u) {
                for (Graph::Vertex v : graph.GetNeighbours(u)) {
                    for (FlightTime e: graph.GetEdgeWeight(u, v)) {
                        if (e.from >= dist[u] && e.to < dist[v]) {
                            dist[v] = e.to;
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
    WeightedAdjListGraph<FlightTime> graph(vertex_count, true);
    for (size_t i = 0; i < edge_count; ++i) {
        size_t from, to;
        int dep_time, arr_time;
        std::cin >> from >> dep_time >> to >> arr_time;
        graph.AddEdge(from - 1, to - 1, FlightTime(dep_time, arr_time));
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

template <class T>
void WeightedAdjListGraph<T>::AddEdge(const Graph::Vertex &from, const Graph::Vertex &to, T edge_weight) {
    AdjListGraph::AddEdge(from, to);
    weight[{from, to}].push_back(edge_weight);
    if (!directed) {
        weight[{to, from}].push_back(edge_weight);
    }
}

template <class T>
std::vector<T> WeightedAdjListGraph<T>::GetEdgeWeight(const Graph::Vertex &from, const Graph::Vertex &to) const {
    return weight.at(std::make_pair(from, to));
}

