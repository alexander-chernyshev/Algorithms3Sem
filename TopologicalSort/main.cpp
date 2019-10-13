#include <iostream>
#include <vector>
#include <algorithm>

class Graph {
protected:
    size_t vertex_count;
    size_t edge_count = 0;
    bool directed;
public:
    typedef size_t Vertex;
    Graph(size_t _vertex_count, bool _directed);

    virtual std::vector<Vertex> GetNeighbours(const Vertex& v) const = 0;
    virtual void AddEdge(const Vertex& from, const Vertex& to) = 0;

    size_t GetVertexCount() const;
    bool IsDirected() const;
};

class AdjListGraph : public Graph{
private:
    std::vector<std::vector<Vertex>> adj_list;
public:
    AdjListGraph(size_t _vertex_count, bool _directed);

    std::vector<Vertex> GetNeighbours(const Vertex& v) const override;
    void AddEdge(const Vertex& from, const Vertex& to) override;
};

class MatrixGraph : public Graph {
private:
    std::vector<std::vector<size_t>> has_edge;
public:
    MatrixGraph(size_t _vertex_count, bool _directed);

    std::vector<Vertex> GetNeighbours(const Vertex& v) const override;
    void AddEdge(const Vertex& from, const Vertex& to) override;
};

namespace GraphProcessing {
    enum VertexMark {WHITE, GREY, BLACK};

    void dfs(const Graph& g, Graph::Vertex v, std::vector<VertexMark>& color, std::vector<Graph::Vertex>& order, int& err) {
        color[v] = GREY;
        for (Graph::Vertex& u : g.GetNeighbours(v)) {
            if (color[u] == WHITE) {
                dfs(g, u, color, order, err);
            } else if (color[u] == GREY) {
                err = 1;
                return;
            }
        }
        color[v] = BLACK;
        order.push_back(v);
    }

    std::vector<Graph::Vertex> TopologicalSort(Graph& g) {
        if (!g.IsDirected()) { // в случае неориентированного графа можно вернуть любой порядок вершин :)
            std::vector<Graph::Vertex> order(g.GetVertexCount());
            for (size_t i = 0; i < g.GetVertexCount(); ++i) {
                order[i] = i;
            }
            return order;
        }
        std::vector<VertexMark> color(g.GetVertexCount(), WHITE);
        std::vector<Graph::Vertex> order;
        int err = 0;
        for (Graph::Vertex v = 0; v < g.GetVertexCount(); ++v) {
            if (color[v] == WHITE) {
                dfs(g, v, color, order, err);
                if (err) {
                    return std::vector<Graph::Vertex>();
                }
            }
        }
        std::reverse(order.begin(), order.end());
        return order;
    };

}

int main() {
    size_t vertex_count, edge_count;
    std::cin >> vertex_count >> edge_count;
    AdjListGraph graph(vertex_count, true);
    for (size_t i = 0; i < edge_count; ++i) {
        Graph::Vertex from, to;
        std::cin >> from >> to;
        graph.AddEdge(from - 1, to - 1);
    }
    std::vector<Graph::Vertex> order = GraphProcessing::TopologicalSort(graph);
    if (order.empty()) {
        std::cout << "No" << std::endl;
    } else {
        std::cout << "Yes\n";
        for (Graph::Vertex& v : order) {
            std::cout << v + 1 << ' ';
        }
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

bool Graph::IsDirected() const {
    return directed;
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

MatrixGraph::MatrixGraph(size_t _vertex_count, bool _directed): Graph(_vertex_count, _directed) {
    has_edge = std::vector<std::vector<size_t>>(vertex_count);
    for (Vertex i = 0; i < vertex_count; ++i)
        has_edge[i].resize(vertex_count);
}

std::vector<Graph::Vertex> MatrixGraph::GetNeighbours(const Graph::Vertex &v) const {
    std::vector<Graph::Vertex> neighbours;
    for (Vertex i = 0; i < vertex_count; ++i) {
        if (has_edge[v][i] != 0)
            neighbours.push_back(i);
    }
    return neighbours;
}

void MatrixGraph::AddEdge(const Graph::Vertex &from, const Graph::Vertex &to) {
    ++edge_count;
    has_edge[from][to] = 1;
    if (!directed) {
        has_edge[to][from] = 1;
    }
}