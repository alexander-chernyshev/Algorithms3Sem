#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <set>
#include <algorithm>
#include <utility>

const size_t MAX_WEIGHT = 2009000999;

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
    bool operator()(std::pair<size_t, size_t> a, std::pair<size_t, size_t> b) {
        return a.second > b.second;
    }
};

class WeightInterface {
protected:
    std::map<std::pair<Graph::Vertex, Graph::Vertex>, size_t> weight;
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

    void AddEdge(const Vertex &from, const Vertex &to, size_t edge_weight);

    size_t GetEdgeWeight(Vertex from, Vertex to) const;

    void SetEdgeWeight(const Graph::Vertex &from, const Graph::Vertex &to, size_t edge_weight);
};

namespace GraphProcessing {

    std::vector<size_t> Dijkstra(const WeightedAdjListGraph &graph, Graph::Vertex start) {
        std::vector<Graph::Vertex> dist(graph.GetVertexCount(), MAX_WEIGHT);
        std::vector<Graph::Vertex> visited(graph.GetVertexCount(), false);
        dist[start] = 0;
        std::priority_queue<std::pair<Graph::Vertex, size_t>,
                std::vector<std::pair<Graph::Vertex, size_t>>, pair_cmp> queue;
        queue.push({start, 0});
        while (!queue.empty()) {
            Graph::Vertex current = queue.top().first;
            queue.pop();
            if (visited[current]) {
                continue;
            }
            visited[current] = true;
            for (Graph::Vertex u : graph.GetNeighbours(current)) {
                size_t edge_weight = graph.GetEdgeWeight(current, u);
                if (dist[u] > dist[current] + edge_weight) {
                    dist[u] = dist[current] + edge_weight;
                    queue.push({u, dist[u]});
                }
            }
        }
        return dist;
    }
}

void CalculateDistances(size_t num) {
    for (size_t i = 0; i < num; ++i) {
        size_t vertex_count, edge_count;
        std::cin >> vertex_count >> edge_count;
        WeightedAdjListGraph graph(vertex_count, false);
        for (size_t j = 0; j < edge_count; ++j) {
            size_t from, to, edge_weight;
            std::cin >> from >> to >> edge_weight;
            graph.AddEdge(from, to, edge_weight);
        }
        size_t start_vertex;
        std::cin >> start_vertex;
        std::vector<size_t> distances = GraphProcessing::Dijkstra(graph, start_vertex);
        for (size_t distance : distances) {
            std::cout << distance << ' ';
        }
        std::cout << '\n';
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    size_t num;
    std::cin >> num;
    CalculateDistances(num);
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

void WeightedAdjListGraph::AddEdge(const Graph::Vertex &from, const Graph::Vertex &to, size_t edge_weight) {
    AdjListGraph::AddEdge(from, to);
    if (weight.find({from, to}) == weight.end()) {
        weight[{from, to}] = edge_weight;
        if (!directed) {
            weight[{to, from}] = edge_weight;
        }
    } else {
        if (edge_weight < weight[{from, to}]) {
            weight[{from, to}] = edge_weight;
            if (!directed) {
                weight[{to, from}] = edge_weight;
            }
        }
    }
}

size_t WeightedAdjListGraph::GetEdgeWeight(Graph::Vertex from, Graph::Vertex to) const {
    return weight.at({from, to});
}

void WeightedAdjListGraph::SetEdgeWeight(const Graph::Vertex &from, const Graph::Vertex &to, size_t edge_weight) {
    weight[{from, to}] = edge_weight;
    if (!directed) {
        weight[{to, from}] = edge_weight;
    }
}