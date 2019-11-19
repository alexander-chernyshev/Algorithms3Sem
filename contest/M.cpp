#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

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
    const int ROOT_VERTEX = -1;
    size_t timer = 0;

    struct DFSParameters {
        std::vector<size_t> tin;
        std::vector<size_t> fup;
        std::vector<VertexMark> color;

        explicit DFSParameters(size_t graph_size) {
            tin.resize(graph_size);
            fup.resize(graph_size);
            color.assign(graph_size, WHITE);
        }
    };

    void DFS(const Graph &g, DFSParameters &dfs_parameters, std::set<Graph::Vertex> &a_points,
             Graph::Vertex v, Graph::Vertex prev = ROOT_VERTEX) {
        dfs_parameters.color[v] = GREY;
        ++timer;
        dfs_parameters.tin[v] = timer;
        dfs_parameters.fup[v] = timer;
        size_t children_count = 0;
        for (Graph::Vertex &u : g.GetNeighbours(v)) {
            if (u != prev) {
                if (dfs_parameters.color[u] != WHITE) {
                    dfs_parameters.fup[v] = std::min(dfs_parameters.fup[v], dfs_parameters.tin[u]);
                } else {
                    DFS(g, dfs_parameters, a_points, u, v);
                    ++children_count;
                    dfs_parameters.fup[v] = std::min(dfs_parameters.fup[v], dfs_parameters.fup[u]);
                    if (dfs_parameters.fup[u] >= dfs_parameters.tin[v] && prev != -1) {
                        a_points.insert(v);
                    }
                }
            }
        }
        if (prev == -1 && children_count > 1)
            a_points.insert(v);
        dfs_parameters.color[v] = BLACK;
    }

    std::set<Graph::Vertex> GetArticulationPoints(const Graph &graph) {
        DFSParameters dfs_parameters(graph.GetVertexCount());
        std::set<Graph::Vertex> a_points;
        for (Graph::Vertex i = 0; i < graph.GetVertexCount(); ++i) {
            if (dfs_parameters.color[i] == WHITE) {
                DFS(graph, dfs_parameters, a_points, i);
            }
        }
        return a_points;
    }
}


int main() {
    size_t vert_count, edge_count;
    std::cin >> vert_count >> edge_count;
    AdjListGraph graph(vert_count, false);
    for (size_t i = 0; i < edge_count; ++i) {
        size_t from, to;
        std::cin >> from >> to;
        graph.AddEdge(from - 1, to - 1);
    }
    std::set<Graph::Vertex> a_points = GraphProcessing::GetArticulationPoints(graph);
    std::cout << a_points.size() << '\n';
    for (Graph::Vertex v : a_points) {
        std::cout << v + 1 << '\n';
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
