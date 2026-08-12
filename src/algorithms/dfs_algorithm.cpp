#include "dfs_algorithm.h"
#include <algorithm>

dfs_algorithm::dfs_algorithm(interface_graph& graph_ref)
    : graph(graph_ref)
{
}

bool dfs_algorithm::is_reachable(int start_id, int target_id)
{
    return find_path(start_id, target_id).reach_able;
}

PathResult dfs_algorithm::find_path(int start_id, int target_id)
{
    PathResult result;

    if (!graph.has_station(start_id) || !graph.has_station(target_id))
        throw out_of_range("invalid station_id");
    
    int station_count = graph.get_station_count();

    vector<bool> visited(station_count, false);
    vector<int> parent(station_count, -1);

    dfs_traversal traversal;

    auto get_neighbors = [this](int current_id) -> vector<int>{
        vector<int> neighbors;

        for (auto& e : graph.get_neighbors(current_id))
            neighbors.push_back(e.get_destination());

        return neighbors;
    };

    bool found = false;

    auto on_enter = [&](int current_id){
        if (current_id == target_id)
            found = true;
    };

    traversal.traverse(start_id, visited, parent, get_neighbors, on_enter, nullptr, nullptr, nullptr);

    if (!found)
        return result;

    vector<int> path;

    for (int at = target_id; at != -1; at = parent[at])
        path.push_back(at);
    

    reverse(path.begin(), path.end());

    result.reach_able = true;
    result.path = path;
    result.total_cost = static_cast<double>(path.size() - 1);

    return result;
}