//
// Created by alexander on 13.09.2019.
//

#include "AdjListGraph.h"

AdjListGraph::AdjListGraph(size_t _vertex_count, bool _is_oriented)
        : Graph(_vertex_count, _is_oriented) {
    adj_list = std::vector<std::vector<Graph::Vertex>>(vertex_count);
}

void AdjListGraph::AddEdge(const Graph::Vertex &from, const Graph::Vertex &to) {
    adj_list[from].push_back(to);
    if (!is_oriented)
        adj_list[to].push_back(from);
}

std::vector<Graph::Vertex> AdjListGraph::GetNeighbours(const Graph::Vertex &v) const{
    return adj_list[v];
}