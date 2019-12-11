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

    virtual void AddEdge(const Graph::Vertex &from, const Graph::Vertex &to, long long int edge_weight) = 0;

    virtual long long int GetEdgeWeight(const Graph::Vertex &from, const Graph::Vertex &to) const = 0;

    virtual void SetEdgeWeight(const Graph::Vertex &from, const Graph::Vertex &to, long long int edge_weight) = 0;
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
    std::vector<std::vector<long long int>> weight;
public:
    WeightedAdjListGraph(size_t _vertex_count, bool _directed) : Graph(_vertex_count, _directed),
                                                                 AdjListGraph(_vertex_count, _directed),
                                                                 WeightedGraph(_vertex_count, _directed),
                                                                 weight(_vertex_count) {
        for (auto &v : weight) {
            v.resize(_vertex_count);
        }
    };

    void AddEdge(const Vertex &from, const Vertex &to, long long int edge_weight) override;

    long long int GetEdgeWeight(const Vertex &from, const Vertex &to) const override;

    void SetEdgeWeight(const Graph::Vertex &from, const Graph::Vertex &to, long long int edge_weight) override;
};

template<class T>
std::vector<std::vector<T>> make_square_matrix(size_t size) {
    std::vector<std::vector<T>> matrix(size);
    for (auto &v : matrix) {
        v.resize(size);
    }
    return matrix;
}

namespace GraphProcessing {
    int FindIncreasingFlow(const WeightedGraph &graph, Graph::Vertex start, Graph::Vertex finish,
                           std::vector<std::vector<long long int>> &res_network) {
        std::queue<Graph::Vertex> q;
        std::vector<bool> visited(graph.GetVertexCount());
        std::vector<long long int> max_res_flow(graph.GetVertexCount());
        std::vector<Graph::Vertex> prev(graph.GetVertexCount(), -1);
        q.push(start);
        max_res_flow[start] = MAX_FLOW;
        while (!q.empty()) {
            Graph::Vertex cur_v = q.front();
            q.pop();
            visited[cur_v] = true;
            for (Graph::Vertex v = 0; v < graph.GetVertexCount(); ++v) {
                if (!visited[v] && res_network[cur_v][v] > 0) {
                    q.push(v);
                    prev[v] = cur_v;
                    max_res_flow[v] = std::min(max_res_flow[cur_v], res_network[cur_v][v]);
                }
                if (v == finish) {
                    break;
                }
            }
        }
        int flow = max_res_flow[finish];
        if (flow == 0) {
            return 0;
        }
        Graph::Vertex current = finish;
        while (prev[current] != -1) {
            res_network[prev[current]][current] -= flow;
            res_network[current][prev[current]] += flow;
            current = prev[current];
        }
        return flow;
    }

    int EdmondsKarpMaxFlow(const WeightedGraph &g, Graph::Vertex start, Graph::Vertex finish) {
        std::vector<std::vector<long long int>> residual_network = make_square_matrix<long long int>(
                g.GetVertexCount());
        for (Graph::Vertex u = 0; u < g.GetVertexCount(); ++u) {
            for (auto v : g.GetNeighbours(u)) {
                residual_network[u][v] = g.GetEdgeWeight(u, v);
            }
        }
        int delta = MAX_FLOW;
        int graph_flow = 0;
        while (delta > 0) {
            delta = FindIncreasingFlow(g, start, finish, residual_network);
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
    std::cout << GraphProcessing::EdmondsKarpMaxFlow(graph, 0, vertex_count - 1);
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

void WeightedAdjListGraph::AddEdge(const Graph::Vertex &from, const Graph::Vertex &to, long long int edge_weight) {
    AdjListGraph::AddEdge(from, to);
    weight[from][to] += edge_weight;
    if (!directed) {
        weight[from][to] += edge_weight;
    }
}

long long int WeightedAdjListGraph::GetEdgeWeight(const Graph::Vertex &from, const Graph::Vertex &to) const {
    return weight[from][to];
}

void
WeightedAdjListGraph::SetEdgeWeight(const Graph::Vertex &from, const Graph::Vertex &to, long long int edge_weight) {
    weight[from][to] = edge_weight;
    if (!directed) {
        weight[from][to] = edge_weight;
    }
}