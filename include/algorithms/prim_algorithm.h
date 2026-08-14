#pragma once


#include "interface_graph.h"
#include "route_metric.h"
#include "mst_result.h"

class prim_algorithm
{
public:
    explicit prim_algorithm(interface_graph &graph_ref);

    MSTResult build_mst(route_metric metric);

private:
    interface_graph &graph;

    double get_edge_weight(edge &e, route_metric metric);
};


