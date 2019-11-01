#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <utility>

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

    std::vector<Edge> PrimFindMST(const WeightedAdjListGraph &graph) {
        std::vector<Edge> mst;
        std::priority_queue<std::pair<Graph::Vertex, size_t>,
                std::vector<std::pair<Graph::Vertex, size_t>>, pair_cmp> queue;
        std::vector<size_t> key(graph.GetVertexCount(), MAX_WEIGHT);
        std::vector<int> prev(graph.GetVertexCount(), ROOT_VERTEX);
        key[0] = 0;
        std::vector<bool> visited(graph.GetVertexCount(), false);
        for (Graph::Vertex i = 0; i < graph.GetVertexCount(); ++i) {
            queue.push({i, key[i]});
        }
        while (!queue.empty()) {
            std::pair<Graph::Vertex, size_t> current_vertex = queue.top();
            queue.pop();
            Graph::Vertex v = current_vertex.first;
            visited[v] = true;
            for (Graph::Vertex u : graph.GetNeighbours(v)) {
                if (!visited[u] && key[u] > graph.GetEdgeWeight(v, u)) {
                    prev[u] = v;
                    key[u] = graph.GetEdgeWeight(v, u);
                    queue.push({u, key[u]});
                }
            }
        }
        for (Graph::Vertex v = 0; v < graph.GetVertexCount(); ++v) {
            if (prev[v] != ROOT_VERTEX) {
                Edge e(prev[v], v, graph.GetEdgeWeight(prev[v], v));
                mst.push_back(e);
            }
        }
        return mst;
    }

    size_t GetMSTWeight(const std::vector<Edge> &mst) {
        size_t tree_weight = 0;
        for (Edge e : mst) {
            tree_weight += e.weight;
        }
        return tree_weight;
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    size_t vertex_count;
    std::cin >> vertex_count;
    WeightedAdjListGraph graph(vertex_count + 1, false);
    for (size_t i = 0; i < vertex_count; ++i) {
        for (size_t j = 0; j < vertex_count; ++j) {
            int cost;
            std::cin >> cost;
            if (i != j) {
                graph.AddEdge(i, j, cost);
            }
        }
    }
    for (Graph::Vertex i = 0; i < vertex_count; ++i) {
        int cost;
        std::cin >> cost;
        graph.AddEdge(i, vertex_count, cost);
    }
    std::vector<Edge> mst = GraphProcessing::PrimFindMST(graph);
    std::cout << GraphProcessing::GetMSTWeight(mst);
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