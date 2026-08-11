#include "bfs_algorithm.h"
#include <queue>
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

    int station_count = graph.get_station_count();
    vector<bool> visited(station_count, false);
    vector<int> parent(station_count, -1);

    queue<int> my_queue;
    my_queue.push(start_id);
    visited[start_id] = true;

    while (!my_queue.empty())
    {
        int current = my_queue.front();
        my_queue.pop();

        if (current == target_id)
        {
            break;
        }

        for (auto& e : graph.get_neighbors(current))
        {
            int neighbor = e.get_destination();
            if (!visited[neighbor])
            {
                visited[neighbor] = true;
                parent[neighbor] = current;
                my_queue.push(neighbor);
            }
        }
    }

    if (!visited[target_id])
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
