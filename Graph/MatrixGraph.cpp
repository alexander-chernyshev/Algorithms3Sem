//
// Created by alexander on 16.09.2019.
//

#include "MatrixGraph.h"

MatrixGraph::MatrixGraph(size_t _vertex_count, bool _is_oriented): Graph(_vertex_count, _is_oriented) {
    matrix = std::vector<std::vector<size_t>>(vertex_count);
    for (size_t i = 0; i < vertex_count; ++i)
        matrix[i].resize(vertex_count);
}

std::vector<Graph::Vertex> MatrixGraph::GetNeighbours(const Graph::Vertex &v) const {
    std::vector<Graph::Vertex> neighbours;
    for (size_t i = 0; i < vertex_count; ++i) {
        if (matrix[v][i] != 0)
            neighbours.push_back(i);
    }
    return neighbours;
}

void MatrixGraph::AddEdge(const Graph::Vertex &from, const Graph::Vertex &to) {
    ++edge_count;
    matrix[from][to] = 1;
}
