#ifndef dijkstra_algorithm_h
#define dijkstra_algorithm_h

#include "interface_graph.h"
#include "station.h"
#include "edge.h"
#include "path_result.h"
#include "route_metric.h"

class dijkstra_algorithm
{
public:
    dijkstra_algorithm(interface_graph& graph_ref);

    PathResult find_shortest_path(int start_id, int target_id, route_metric metric);

private:
    interface_graph& graph;

     double get_edge_weight(edge& e, route_metric metric);
};

#endif
