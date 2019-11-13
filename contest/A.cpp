#include <iostream>
#include <vector>

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
};

class AdjListGraph : public Graph {
private:
    std::vector<std::vector<Vertex>> adj_list;
public:
    AdjListGraph(size_t _vertex_count, bool _directed);

    std::vector<Vertex> GetNeighbours(const Vertex &v) const override;

    void AddEdge(const Vertex &from, const Vertex &to) override;
};

namespace GraphProcessing {
    typedef std::vector<Graph::Vertex> ConnectedComponent;
    enum VertexMark {
        WHITE, GREY, BLACK
    };

    void GetVerticesInSameComponent(const Graph &g, std::vector<VertexMark> &color, const Graph::Vertex& v,
                                    ConnectedComponent &component) {
        color[v] = GREY;
        component.push_back(v);
        for (Graph::Vertex &u : g.GetNeighbours(v)) {
            if (color[u] == WHITE) {
                GetVerticesInSameComponent(g, color, u, component);
            }
        }
        color[v] = BLACK;
    }

    ConnectedComponent GetComponent(const Graph &g, const Graph::Vertex& v) {
        size_t vertex_count = g.GetVertexCount();
        ConnectedComponent component;
        std::vector<VertexMark> color(vertex_count, WHITE);
        GetVerticesInSameComponent(g, color, v, component);
        return component;
    }

    std::vector<ConnectedComponent> GetConnectedComponents(const Graph &g) {
        size_t vertex_count = g.GetVertexCount();
        std::vector<ConnectedComponent> components;
        std::vector<VertexMark> color(vertex_count, WHITE);
        for (Graph::Vertex v = 0; v < vertex_count; ++v) {
            if (color[v] == WHITE) {
                components.back();
                GetVerticesInSameComponent(g, color, v, components[components.size()-1]);
            }
        }
        return components;
    }
}

int main() {
    size_t vert_count, edge_count;
    std::cin >> vert_count >> edge_count;
    AdjListGraph graph(vert_count, false);
    for (size_t i = 0; i < edge_count; ++i) {
        Graph::Vertex from, to;
        std::cin >> from >> to;
        graph.AddEdge(from - 1, to - 1);
    }
    std::vector<GraphProcessing::ConnectedComponent> components = GraphProcessing::GetConnectedComponents(graph);
    std::cout << components.size() << '\n';
    for (auto& component : components) {
        std::cout << component.size() << '\n';
        for (auto v : component) {
            std::cout << v + 1 << ' ';
        }
        std::cout << '\n';
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