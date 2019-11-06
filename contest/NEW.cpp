#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <set>
#include <algorithm>
#include <utility>
#include <cinttypes>
#include <climits>

const uint64_t MAX_WEIGHT = std::numeric_limits<uint64_t>::max();

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
    std::map<std::pair<Graph::Vertex, Graph::Vertex>, uint64_t> weight;
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

    void AddEdge(const Vertex &from, const Vertex &to, uint64_t edge_weight);

    uint64_t GetEdgeWeight(Vertex from, Vertex to) const;

    void SetEdgeWeight(const Graph::Vertex &from, const Graph::Vertex &to, uint64_t edge_weight);
};

namespace GraphProcessing {

    std::vector<uint64_t> Dijkstra(const WeightedAdjListGraph &graph, const std::vector<Graph::Vertex> &start_vertices) {
        std::vector<uint64_t> dist(graph.GetVertexCount(), MAX_WEIGHT);
        std::vector<Graph::Vertex> visited(graph.GetVertexCount(), false);
        std::priority_queue<std::pair<Graph::Vertex, uint64_t>,
                std::vector<std::pair<Graph::Vertex, uint64_t>>, pair_cmp> queue;
        for (Graph::Vertex v : start_vertices) {
            dist[v] = 0;
            queue.push({v, 0});
        }
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

uint64_t FindSafePathLength(const WeightedAdjListGraph &graph,
                          Graph::Vertex start,
                          Graph::Vertex finish,
                          const std::vector<uint64_t> &fire_time) {
    std::vector<uint64_t> dist(graph.GetVertexCount(), MAX_WEIGHT);
    std::vector<Graph::Vertex> visited(graph.GetVertexCount(), false);
    dist[start] = 0;
    std::priority_queue<std::pair<Graph::Vertex, uint64_t>,
            std::vector<std::pair<Graph::Vertex, uint64_t>>, pair_cmp> queue;
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
            if (dist[u] > dist[current] + edge_weight && dist[current] + edge_weight < fire_time[u]) {
                dist[u] = dist[current] + edge_weight;
                queue.push({u, dist[u]});
            }
        }
    }
    return dist[finish];
}

int main() {
    size_t vertex_count, edge_count, fire_count;
    std::cin >> vertex_count >> edge_count >> fire_count;
    std::vector<size_t> fire_vertices(fire_count);
    for (size_t& v : fire_vertices) {
        std::cin >> v;
        --v;
    }
    WeightedAdjListGraph graph(vertex_count, false);
    for (size_t i = 0; i < edge_count; ++i) {
        size_t from, to;
        uint64_t distance;
        std::cin >> from >> to >> distance;
        graph.AddEdge(from-1, to-1, distance);
    }
    size_t start, finish;
    std::cin >> start >> finish;
    std::vector<uint64_t> fire_time = GraphProcessing::Dijkstra(graph, fire_vertices);
    uint64_t escape_time = FindSafePathLength(graph, start-1, finish-1, fire_time);
    if (escape_time == MAX_WEIGHT) {
        std::cout << -1;
    } else {
        std::cout << escape_time;
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

void WeightedAdjListGraph::AddEdge(const Graph::Vertex &from, const Graph::Vertex &to, uint64_t edge_weight) {
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

uint64_t WeightedAdjListGraph::GetEdgeWeight(Graph::Vertex from, Graph::Vertex to) const {
    return weight.at({from, to});
}

void WeightedAdjListGraph::SetEdgeWeight(const Graph::Vertex &from, const Graph::Vertex &to, uint64_t edge_weight) {
    weight[{from, to}] = edge_weight;
    if (!directed) {
        weight[{to, from}] = edge_weight;
    }
}