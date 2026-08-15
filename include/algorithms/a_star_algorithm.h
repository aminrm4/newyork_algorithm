#ifndef A_STAR_ALGORITHM_H
#define A_STAR_ALGORITHM_H

#include "interface_graph.h"
#include "station.h"
#include "edge.h"
#include "path_result.h"
#include "route_metric.h"

class a_star_algorithm
{
public:
    a_star_algorithm(interface_graph &graph_ref);

    PathResult find_shortest_path(int start_id, int target_id, route_metric metric);

private:
    interface_graph &graph;
    double distance_scale;
    double time_scale;

    double get_edge_weight(edge &e, route_metric metric);
    double heuristic(int station_id, int target_id, route_metric metric);
    double haversine_distance(station &from, station &to);
    void calculate_scales();
};

#endif
