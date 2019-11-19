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

    virtual std::set<Vertex> GetNeighbours(const Vertex &v) const = 0;

    virtual void AddEdge(const Vertex &from, const Vertex &to) = 0;

    virtual bool CheckEdge(const Vertex &from, const Vertex &to) const = 0;

    size_t GetVertexCount() const;

    bool IsDirected() const;
};

class WeightedGraph : virtual public Graph{
public:
    WeightedGraph(size_t _vertex_count, bool _directed): Graph(_vertex_count, _directed) {};

    virtual void AddEdge(const Graph::Vertex &from, const Graph::Vertex &to, int edge_weight) = 0;

    virtual int GetEdgeWeight(const Graph::Vertex &from, const Graph::Vertex &to) const = 0;

    virtual void SetEdgeWeight(const Graph::Vertex &from, const Graph::Vertex &to, int edge_weight) = 0;
};

class AdjListGraph : virtual public Graph {
private:
    std::vector<std::set<Vertex>> adj_list;
public:
    AdjListGraph(size_t _vertex_count, bool _directed): Graph(_vertex_count, _directed) {
        adj_list.resize(_vertex_count);
    }

    std::set<Vertex> GetNeighbours(const Vertex &v) const override;

    void AddEdge(const Vertex &from, const Vertex &to) override;

    bool CheckEdge(const Vertex &from, const Vertex &to) const override;
};

class WeightedAdjListGraph : virtual public AdjListGraph, virtual public WeightedGraph {
private:
    std::map<std::pair<Graph::Vertex, Graph::Vertex>, int> weight;
public:
    WeightedAdjListGraph(size_t _vertex_count, bool _directed): Graph(_vertex_count, _directed), AdjListGraph(_vertex_count, _directed), WeightedGraph(_vertex_count, _directed) {};

    void AddEdge(const Vertex &from, const Vertex &to, int edge_weight) override;

    int GetEdgeWeight(const Vertex &from, const Vertex &to) const override;

    void SetEdgeWeight(const Graph::Vertex &from, const Graph::Vertex &to, int edge_weight) override;
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

    struct VertexPriority {
        Graph::Vertex vertex;
        size_t priority;

        VertexPriority(Graph::Vertex _vertex, size_t _priority): vertex(_vertex), priority(_priority) {}
    };

    struct vertex_priority_cmp {
        bool operator()(const VertexPriority& a, const VertexPriority& b) {
            return a.priority > b.priority;
        }
    };

    typedef std::priority_queue<VertexPriority, std::vector<VertexPriority>, vertex_priority_cmp> PriorityQueue;

    std::vector<Edge> PrimFindMST(const WeightedGraph &graph) {
        std::vector<Edge> mst;
        PriorityQueue queue;
        std::vector<size_t> priority(graph.GetVertexCount(), MAX_WEIGHT);
        std::vector<int> prev(graph.GetVertexCount(), ROOT_VERTEX);
        priority[0] = 0;
        std::vector<bool> visited(graph.GetVertexCount(), false);
        for (Graph::Vertex i = 0; i < graph.GetVertexCount(); ++i) {
            queue.push(VertexPriority(i, priority[i]));
        }
        while (!queue.empty()) {
            Graph::Vertex v = queue.top().vertex;
            queue.pop();
            visited[v] = true;
            for (Graph::Vertex u : graph.GetNeighbours(v)) {
                if (!visited[u] && priority[u] > graph.GetEdgeWeight(v, u)) {
                    prev[u] = v;
                    priority[u] = graph.GetEdgeWeight(v, u);
                    queue.push(VertexPriority(u, priority[u]));
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
    size_t vertex_count, edge_count;
    std::cin >> vertex_count >> edge_count;
    WeightedAdjListGraph graph(vertex_count, false);
    for (size_t i = 0; i < edge_count; ++i) {
        size_t from, to, weight;
        std::cin >> from >> to >> weight;
        graph.AddEdge(from - 1, to - 1, weight);
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

void AdjListGraph::AddEdge(const Graph::Vertex &from, const Graph::Vertex &to) {
    ++edge_count;
    adj_list[from].insert(to);
    if (!directed) {
        adj_list[to].insert(from);
    }
}

bool AdjListGraph::CheckEdge(const Graph::Vertex &from, const Graph::Vertex &to) const {
    return !(adj_list[from].find(to) == adj_list[from].end());
}

std::set<Graph::Vertex> AdjListGraph::GetNeighbours(const Graph::Vertex &v) const {
    return adj_list[v];
}

void WeightedAdjListGraph::AddEdge(const Graph::Vertex &from, const Graph::Vertex &to, int edge_weight) {
    if (!CheckEdge(from, to)) {
        AdjListGraph::AddEdge(from, to);
        weight[{from, to}] = edge_weight;
        if (!directed) {
            weight[{to, from}] = edge_weight;
        }
    } else {
        if (edge_weight < GetEdgeWeight(from, to)) {
            SetEdgeWeight(from, to, edge_weight);
        }
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