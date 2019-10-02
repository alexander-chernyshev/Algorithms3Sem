//
// Created by alexander on 16.09.2019.
//
#include "Graph.h"
#include <vector>
#include <iostream>

#ifndef GRAPH_MATRIXGRAPH_H
#define GRAPH_MATRIXGRAPH_H



class MatrixGraph : public Graph {
private:
    std::vector<std::vector<size_t>> has_edge;
public:
    MatrixGraph(size_t _vertex_count, bool _directed);

    std::vector<Vertex> GetNeighbours(const Vertex& v) const override;
    void AddEdge(const Vertex& from, const Vertex& to) override;
};


#endif //GRAPH_MATRIXGRAPH_H
