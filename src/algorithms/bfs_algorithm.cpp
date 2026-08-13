#include "bfs_algorithm.h"
#include <algorithm>

bfs_algorithm::bfs_algorithm(interface_graph& graph_ref): graph(graph_ref)
{
}

bool bfs_algorithm::is_reachable(int start_id, int target_id)
{
    return find_path(start_id, target_id).reach_able;
}

PathResult bfs_algorithm::find_path(int start_id, int target_id)
{
    PathResult result;

    if(!graph.has_station(start_id) || !graph.has_station(target_id))
        throw out_of_range("invalid station id");

    int station_count = graph.get_station_count();
    bfs_traversal traversal;
    vector<int> parent;

    auto get_neighbors = [this](int current_id) -> vector<int> {
        vector<int> result;

        for (auto& e : graph.get_neighbors(current_id)){
            result.push_back(e.get_destination());
        }
        return result;
    };

    bool found = traversal.search(start_id, target_id, station_count, parent, get_neighbors);

    if(!found) return result;

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
