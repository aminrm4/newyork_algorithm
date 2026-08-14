#ifndef bellman_ford_algorithm_h
#define bellman_ford_algorithm_h

#include "weighted_digraph.h"
#include "bellman_ford_result.h"


class bellman_ford_algorithm
{
public:
    explicit bellman_ford_algorithm(weighted_digraph &graph_ref);

    BellmanFordResult find_shortest_path(int start_id, int target_id);

private:
    weighted_digraph &graph;
};

#endif
