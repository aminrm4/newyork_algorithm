#ifndef dfs_algorithm_h
#define dfs_algorithm_h

#include "interface_graph.h"
#include "station.h"
#include "edge.h"
#include "path_result.h"

class dfs_algorithm
{
public:
    dfs_algorithm(interface_graph& graph_ref);

    bool is_reachable(int start_id, int target_id);
    PathResult find_path(int start_id, int target_id);

private:
    interface_graph& graph;

    bool visit(int current_id, int target_id, vector<bool>& visited, vector<int>& parent);
};

#endif
