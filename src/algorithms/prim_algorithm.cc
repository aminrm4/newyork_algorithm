#include "prim_algorithm.h"
#include <queue>
#include <vector>

prim_algorithm::prim_algorithm(interface_graph &graph_ref) : graph(graph_ref)
{
}

double prim_algorithm::get_edge_weight(edge &e, route_metric metric)
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

MSTResult prim_algorithm::build_mst(route_metric metric)
{
    MSTResult result;

    int station_count = graph.get_station_count();
    vector<bool> visited(station_count, false);

    auto by_weight_ascending = [](const MSTEdge &a, const MSTEdge &b)
    { return a.weight > b.weight; };
    priority_queue<MSTEdge, vector<MSTEdge>, decltype(by_weight_ascending)> pending(by_weight_ascending);

    for (int i = 0; i < station_count; i++)
    {
        if (visited[i])
        {
            continue;
        }

        visited[i] = true;

        for (auto &e : graph.get_neighbors(i))
        {
            pending.emplace(i, e.get_destination(), get_edge_weight(e, metric));
        }

        while (!pending.empty())
        {
            MSTEdge candidate_edge = pending.top();
            pending.pop();

            int from = candidate_edge.from;
            int to = candidate_edge.to;
            double weight = candidate_edge.weight;

            if (visited[to])
            {
                continue;
            }

            visited[to] = true;
            result.edges.emplace_back(from, to, weight);
            result.total_weight += weight;

            for (auto &e : graph.get_neighbors(to))
            {
                int neighbor = e.get_destination();
                if (!visited[neighbor])
                {
                    pending.emplace(to, neighbor, get_edge_weight(e, metric));
                }
            }
        }

        result.component_count++;
    }

    result.is_spanning_tree = (station_count > 0) && (result.component_count == 1);

    return result;
}
