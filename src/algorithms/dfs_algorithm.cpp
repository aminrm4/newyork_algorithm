#include "dfs_algorithm.h"
#include <algorithm>

dfs_algorithm::dfs_algorithm(interface_graph& graph_ref): graph(graph_ref)
{
}

bool dfs_algorithm::is_reachable(int start_id, int target_id)
{
    return find_path(start_id, target_id).reach_able;
}

PathResult dfs_algorithm::find_path(int start_id, int target_id)
{
    PathResult result;

    int station_count = graph.get_station_count();
    vector<bool> visited(station_count, false);
    vector<int> parent(station_count, -1);

    visited[start_id] = true;
    bool found = visit(start_id, target_id, visited, parent);

    if (!found)
    {
        return result;
    }

    vector<int> path;
    for (int at = target_id; at != -1; at = parent[at])
    {
        path.push_back(at);
    }
    reverse(path.begin(), path.end());

    result.reach_able = true;
    result.path = path;
    result.total_cost = static_cast<double>(path.size() - 1);

    return result;
}

bool dfs_algorithm::visit(int current_id, int target_id, vector<bool>& visited, vector<int>& parent)
{
    if (current_id == target_id)
    {
        return true;
    }

    for (auto& e : graph.get_neighbors(current_id))
    {
        int neighbor = e.get_destination();
        if (!visited[neighbor])
        {
            visited[neighbor] = true;
            parent[neighbor] = current_id;

            if (visit(neighbor, target_id, visited, parent))
            {
                return true;
            }
        }
    }

    return false;
}
