#include "bellman_ford_algorithm.h"
#include <limits>
#include <algorithm>
#include <stdexcept>

bellman_ford_algorithm::bellman_ford_algorithm(weighted_digraph &graph_ref) : graph(graph_ref)
{
}

BellmanFordResult bellman_ford_algorithm::find_shortest_path(int start_id, int target_id)
{
    BellmanFordResult result;

    if (!graph.has_vertex(start_id) || !graph.has_vertex(target_id))
    {
        throw out_of_range("invalid station_id");
    }

    int vertex_count = graph.get_vertex_count();
    double infinity = numeric_limits<double>::infinity();

    vector<double> best_cost(vertex_count, infinity);
    vector<int> parent(vertex_count, -1);

    best_cost[start_id] = 0.0;

    auto all_edges = graph.get_all_edges();

    for (int iteration = 0; iteration < vertex_count - 1; iteration++)
    {
        bool relaxed_any = false;

        for (auto &current_edge : all_edges)
        {
            int from = current_edge.from;
            int to = current_edge.to;
            double weight = current_edge.weight;

            if (best_cost[from] == infinity)
            {
                continue;
            }

            double new_cost = best_cost[from] + weight;
            if (new_cost < best_cost[to])
            {
                best_cost[to] = new_cost;
                parent[to] = from;
                relaxed_any = true;
            }
        }
        if (!relaxed_any)
        {
            break;
        }
    }
    
    for (auto &current_edge : all_edges)
    {
        int from = current_edge.from;
        int to = current_edge.to;
        double weight = current_edge.weight;

        if (best_cost[from] == infinity)
        {
            continue;
        }

        if (best_cost[from] + weight < best_cost[to])
        {
            result.negative_cycle_detected = true;
            return result;
        }
    }

    if (best_cost[target_id] == infinity)
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
    result.total_cost = best_cost[target_id];

    return result;
}
