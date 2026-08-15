#include "dijkstra_algorithm.h"
#include <queue>
#include <limits>
#include <algorithm>
#include <utility>

dijkstra_algorithm::dijkstra_algorithm(interface_graph &graph_ref) : graph(graph_ref)
{
}

double dijkstra_algorithm::get_edge_weight(edge &e, route_metric metric)
{
    if (metric == route_metric::DISTANCE)
    {
        return e.get_distance();
    }
    else
    {
        return e.get_time();
    }
}

PathResult dijkstra_algorithm::find_shortest_path(int start_id,int target_id,route_metric metric)
{
    PathResult result;

    if(!graph.has_station(start_id) || !graph.has_station(target_id)){
        throw out_of_range("invalid station_id");
    }

    int station_count = graph.get_station_count();
    double infinity = numeric_limits<double>::infinity();

    vector<double> best_cost(station_count, infinity);
    vector<int> parent(station_count, -1);
    vector<bool> finalized(station_count, false);

    priority_queue<pair<double, int>,vector<pair<double, int>>,greater<pair<double, int>>>pending;

    best_cost[start_id] = 0.0;
    pending.push(make_pair(0.0, start_id));

    while (!pending.empty())
    {
        pair<double, int> top = pending.top();
        pending.pop();

        double current_cost = top.first;
        int current_id = top.second;

        if (finalized[current_id])
        {
            continue;
        }

        finalized[current_id] = true;
        result.expanded_nodes++;

        if (current_id == target_id)
        {
            break;
        }

        for (auto &e : graph.get_neighbors(current_id))
        {
            int neighbor = e.get_destination();
            double weight = get_edge_weight(e, metric);
            double new_cost = current_cost + weight;

            if (new_cost < best_cost[neighbor])
            {
                best_cost[neighbor] = new_cost;
                parent[neighbor] = current_id;
                pending.push(make_pair(new_cost, neighbor));
            }
        }
    }
    if (best_cost[target_id] == infinity)
        return result;

    vector<int> path;

    for (int at = target_id; at != -1; at = parent[at]){
        path.push_back(at);
    }

    reverse(path.begin(), path.end());

    result.reach_able = true;
    result.path = path;
    result.total_cost = best_cost[target_id];

    return result;
}