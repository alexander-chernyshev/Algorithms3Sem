//
// Created by alexander on 13.09.2019.
//

#ifndef ALGORITHMS3SEM_GRAPH_H
#define ALGORITHMS3SEM_GRAPH_H

#include <iostream>
#include <vector>

namespace GraphProcessing {
    enum VertexMark{WHITE, GREY, BLACK};



}

class Graph {
protected:
    size_t vertex_count;
    size_t edge_count = 0;
    bool is_oriented;
public:
    typedef size_t Vertex;
    Graph(size_t _vertex_count, bool _is_oriented);

    virtual std::vector<Vertex> GetNeighbours(const Vertex& v) const = 0;
    virtual void AddEdge(const Vertex& from, const Vertex& to) = 0;
};


#endif //ALGORITHMS3SEM_GRAPH_H
