//
// Created by alexander on 13.09.2019.
//

#include "Graph.h"
#include <vector>
#include <iostream>

#ifndef ALGORITHMS3SEM_ADJLISTGRAPH_H
#define ALGORITHMS3SEM_ADJLISTGRAPH_H


class AdjListGraph : public Graph{
private:
    std::vector<std::vector<Vertex>> adj_list;
public:
    AdjListGraph(size_t _vertex_count, bool _directed);

    std::vector<Vertex> GetNeighbours(const Vertex& v) const override;
    void AddEdge(const Vertex& from, const Vertex& to) override;
};


#endif //ALGORITHMS3SEM_ADJLISTGRAPH_H
