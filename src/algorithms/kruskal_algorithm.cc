#include "kruskal_algorithm.h"
#include "union_find.h"
#include <algorithm>

kruskal_algorithm::kruskal_algorithm(interface_graph &graph_ref) : graph(graph_ref)
{
}

double kruskal_algorithm::get_edge_weight(edge &e, route_metric metric)
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

MSTResult kruskal_algorithm::build_mst(route_metric metric)
{
    MSTResult result;

    int station_count = graph.get_station_count();
    vector<MSTEdge> candidate_edges;

    for (int station_id = 0; station_id < station_count; station_id++)
    {
        for (auto &e : graph.get_neighbors(station_id))
        {
            int neighbor = e.get_destination();

            if (neighbor > station_id)
            {
                candidate_edges.emplace_back(station_id, neighbor, get_edge_weight(e, metric));
            }
        }
    }

    sort(candidate_edges.begin(), candidate_edges.end(),
         [](const MSTEdge &a, const MSTEdge &b)
         { return a.weight < b.weight; });

    union_find components(station_count);

    for (auto &candidate : candidate_edges)
    {
        if (components.union_sets(candidate.from, candidate.to))
        {
            result.edges.push_back(candidate);
            result.total_weight += candidate.weight;
        }
    }

    result.component_count = components.get_component_count();
    result.is_spanning_tree = (station_count > 0) && (result.component_count == 1);

    return result;
}
