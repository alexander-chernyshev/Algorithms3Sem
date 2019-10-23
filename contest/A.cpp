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
    enum VertexMark {
        WHITE, GREY, BLACK
    };

    void GetVerticesInSameComponent(const Graph &g, std::vector<VertexMark> &color, Graph::Vertex v,
                                    std::vector<Graph::Vertex> &component) {
        color[v] = GREY;
        component.push_back(v);
        for (Graph::Vertex &u : g.GetNeighbours(v)) {
            if (color[u] == WHITE) {
                GetVerticesInSameComponent(g, color, u, component);
            }
        }
        color[v] = BLACK;
    }

    std::vector<Graph::Vertex> GetComponent(const Graph &g, Graph::Vertex v) {
        size_t vertex_count = g.GetVertexCount();
        std::vector<Graph::Vertex> component;
        std::vector<VertexMark> color(vertex_count, WHITE);
        GetVerticesInSameComponent(g, color, v, component);
        return component;
    }

    std::vector<std::vector<Graph::Vertex>> GetAllComponents(const Graph &g) {
        size_t vertex_count = g.GetVertexCount();
        std::vector<std::vector<Graph::Vertex >> components;
        std::vector<VertexMark> color(vertex_count, WHITE);
        for (Graph::Vertex v = 0; v < vertex_count; ++v) {
            if (color[v] == WHITE) {
                components.emplace_back();
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
    std::vector<std::vector<Graph::Vertex >> result = GraphProcessing::GetAllComponents(graph);
    std::cout << result.size() << '\n';
    for (size_t i = 0; i < result.size(); ++i) {
        std::cout << result[i].size() << '\n';
        for (size_t j = 0; j < result[i].size(); ++j) {
            std::cout << result[i][j] + 1 << ' ';
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
