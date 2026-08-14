#pragma once
#include "weighted_digraph.h"
#include "path_result.h"


class dag_shortest_path_algorithm
{
public:
    explicit dag_shortest_path_algorithm(weighted_digraph &graph_ref);

    PathResult find_shortest_path(int start_id, int target_id);

private:
    weighted_digraph &graph;
    bool topological_sort(vector<int> &order);
};
