#pragma once

#include <vector>
#include "directed_edge.h"

struct WeightedEdgeRecord
{
    int from;
    int to;
    double weight;

    WeightedEdgeRecord() : from(-1), to(-1), weight(0.0)
    {
    }

    WeightedEdgeRecord(int from_id, int to_id, double weight_value)
        : from(from_id), to(to_id), weight(weight_value)
    {
    }
};


class weighted_digraph
{
public:
    weighted_digraph();
    explicit weighted_digraph(int vertex_count);
    void resize(int vertex_count);

    void add_edge(int from_id, int to_id, double weight);

    int get_vertex_count();
    bool has_vertex(int vertex_id);
    std::vector<directed_edge> &get_neighbors(int vertex_id);

    std::vector<WeightedEdgeRecord> get_all_edges();

private:
    int vertex_count;
    std::vector<std::vector<directed_edge>> adjacency_list;

    void validate_vertex_id(int vertex_id);
};