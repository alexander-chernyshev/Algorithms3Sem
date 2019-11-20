#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

const int MAX_WEIGHT = 100000000;

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
    std::vector<std::vector<size_t>> MakePathMatrix(const WeightedGraph& graph) {
        std::vector<std::vector<size_t>> matrix(graph.GetVertexCount());
        for (size_t i = 0; i < graph.GetVertexCount(); ++i) {
            matrix[i].assign(graph.GetVertexCount(), MAX_WEIGHT);
        }
        for (Graph::Vertex v = 0; v < graph.GetVertexCount(); ++v) {
            for (Graph::Vertex u : graph.GetNeighbours(v)) {
                matrix[v][u] = graph.GetEdgeWeight(v, u);
            }
        }
        return matrix;
    }

    std::vector<std::vector<size_t>> Floyd(const WeightedGraph& graph) {
        std::vector<std::vector<size_t>> path_matrix = MakePathMatrix(graph);
        for (size_t i = 0; i < graph.GetVertexCount(); ++i) {
            for (size_t u = 0; u < graph.GetVertexCount(); ++u) {
                for (size_t v = 0; v < graph.GetVertexCount(); ++v) {
                    path_matrix[u][v] = std::min(path_matrix[u][v], path_matrix[u][i] + path_matrix[i][v]);
                }
            }
        }
        return path_matrix;
    }

    size_t GetShortestPathLength(const WeightedGraph& graph, Graph::Vertex from, Graph::Vertex to) {
        std::vector<std::vector<size_t>> path_matrix = Floyd(graph);
        return path_matrix[from][to];
    }
}

int main() {
    size_t vertex_count, start, finish;
    std::cin >> vertex_count >> start >> finish;
    WeightedAdjListGraph graph(vertex_count, true);
    for (size_t i = 0; i < vertex_count; ++i) {
        for (size_t j = 0; j < vertex_count; ++j) {
            size_t weight;
            std::cin >> weight;
            if (weight != -1) {
                graph.AddEdge(i, j, weight);
            }
        }
    }
    size_t path_len = GraphProcessing::GetShortestPathLength(graph, start-1, finish-1);
    if (path_len == MAX_WEIGHT) {
        std::cout << -1;
    } else {
        std::cout << path_len;
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