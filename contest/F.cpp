#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>

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

    virtual std::shared_ptr<Graph> GetTransposed() const = 0;

    size_t GetVertexCount() const;
};

class AdjListGraph : public Graph {
private:
    std::vector<std::vector<Vertex>> adj_list;
public:
    AdjListGraph(size_t _vertex_count, bool _directed);

    std::vector<Vertex> GetNeighbours(const Vertex &v) const override;

    void AddEdge(const Vertex &from, const Vertex &to) override;

    std::shared_ptr<Graph> GetTransposed() const override;
};
namespace GraphProcessing {
    enum VertexMark {
        WHITE, GREY, BLACK
    };

    typedef std::vector<Graph::Vertex> StrongConnectedComponent;

    void MakeTimeOutOrder(const Graph &g, std::vector<VertexMark> &color, Graph::Vertex v,
                          std::vector<Graph::Vertex> &time_out_order) {
        color[v] = GREY;
        for (Graph::Vertex &u : g.GetNeighbours(v)) {
            if (color[u] == WHITE) {
                MakeTimeOutOrder(g, color, u, time_out_order);
            }
        }
        time_out_order.push_back(v);
        color[v] = BLACK;
    }

    void GetSCComponent(const std::shared_ptr<Graph> &g, std::vector<VertexMark> &color, Graph::Vertex v,
                        std::vector<Graph::Vertex> &component) {
        color[v] = GREY;
        component.push_back(v);
        for (Graph::Vertex &u : g->GetNeighbours(v)) {
            if (color[u] == WHITE) {
                GetSCComponent(g, color, u, component);
            }
        }
        color[v] = BLACK;
    }

    std::vector<StrongConnectedComponent> GetSCC(const Graph &g) {
        std::vector<VertexMark> color(g.GetVertexCount(), WHITE);
        std::vector<Graph::Vertex> time_out_order;
        std::vector<StrongConnectedComponent> components;
        std::shared_ptr<Graph> g_transposed = g.GetTransposed();
        for (Graph::Vertex v = 0; v < g.GetVertexCount(); ++v) {
            if (color[v] == WHITE) {
                MakeTimeOutOrder(g, color, v, time_out_order);
            }
        }
        std::reverse(time_out_order.begin(), time_out_order.end());
        color.assign(g.GetVertexCount(), WHITE);
        for (Graph::Vertex v : time_out_order) {
            if (color[v] == WHITE) {
                std::vector<Graph::Vertex> component;
                GetSCComponent(g_transposed, color, v, component);
                components.push_back(component);
            }
        }
        return components;
    }
}

int main() {
    size_t vertex_count, edge_count;
    std::cin >> vertex_count >> edge_count;
    AdjListGraph graph(vertex_count, true);
    for (size_t i = 0; i < edge_count; ++i) {
        size_t from, to;
        std::cin >> from >> to;
        graph.AddEdge(from - 1, to - 1);
    }
    std::vector<GraphProcessing::StrongConnectedComponent> components = GraphProcessing::GetSCC(graph);
    std::vector<size_t> component_order(vertex_count);
    for (size_t i = 0; i < components.size(); ++i) {
        for (Graph::Vertex v : components[i]) {
            component_order[v] = i + 1;
        }
    }
    std::cout << components.size() << '\n';
    for (auto i : component_order) {
        std::cout << i << ' ';
    }
    return 0;
}

Graph::Graph(size_t _vertex_count, bool _directed) {
    vertex_count = _vertex_count;
    directed = _directed;
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

std::shared_ptr<Graph> AdjListGraph::GetTransposed() const {
    std::shared_ptr<Graph> transposed = std::make_shared<AdjListGraph>(vertex_count, directed);
    for (Graph::Vertex v = 0; v < vertex_count; ++v) {
        for (Graph::Vertex u: adj_list[v]) {
            transposed->AddEdge(u, v);
        }
    }
    return transposed;
}