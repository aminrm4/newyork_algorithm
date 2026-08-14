#include "dag_shortest_path.h"
#include <queue>
#include <limits>
#include <algorithm>
#include <stdexcept>

dag_shortest_path_algorithm::dag_shortest_path_algorithm(weighted_digraph &graph_ref) : graph(graph_ref)
{
}

bool dag_shortest_path_algorithm::topological_sort(vector<int> &order)
{
    int vertex_count = graph.get_vertex_count();
    vector<int> in_degree(vertex_count, 0);

    for (int u = 0; u < vertex_count; u++)
    {
        for (auto &e : graph.get_neighbors(u))
        {
            in_degree[e.get_destination()]++;
        }
    }

    queue<int> ready;
    for (int u = 0; u < vertex_count; u++)
    {
        if (in_degree[u] == 0)
        {
            ready.push(u);
        }
    }

    order.clear();

    while (!ready.empty())
    {
        int u = ready.front();
        ready.pop();
        order.push_back(u);

        for (auto &e : graph.get_neighbors(u))
        {
            int v = e.get_destination();
            in_degree[v]--;
            if (in_degree[v] == 0)
            {
                ready.push(v);
            }
        }
    }
    return static_cast<int>(order.size()) == vertex_count;
}

PathResult dag_shortest_path_algorithm::find_shortest_path(int start_id, int target_id)
{
    PathResult result;

    if (!graph.has_vertex(start_id) || !graph.has_vertex(target_id))
    {
        throw out_of_range("invalid station_id");
    }

    vector<int> order;
    if (!topological_sort(order))
    {
        throw logic_error("express line graph is not a DAG (a cycle was detected)");
    }

    int vertex_count = graph.get_vertex_count();
    double infinity = numeric_limits<double>::infinity();

    vector<double> best_cost(vertex_count, infinity);
    vector<int> parent(vertex_count, -1);

    best_cost[start_id] = 0.0;

    for (int u : order)
    {
        if (best_cost[u] == infinity)
        {
            continue;
        }

        for (auto &e : graph.get_neighbors(u))
        {
            int v = e.get_destination();
            double new_cost = best_cost[u] + e.get_weight();

            if (new_cost < best_cost[v])
            {
                best_cost[v] = new_cost;
                parent[v] = u;
            }
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
