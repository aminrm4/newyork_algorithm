#ifndef bfs_algorithm_h
#define bfs_algorithm_h

#include "interface_graph.h"
#include "station.h"
#include "edge.h"
#include "path_result.h"
#include "bfs_traversal.h"

class bfs_algorithm
{
public:
    bfs_algorithm(interface_graph& graph_ref);

    bool is_reachable(int start_id, int target_id);
    PathResult find_path(int start_id, int target_id);

private:
    interface_graph& graph;
};

#endif
