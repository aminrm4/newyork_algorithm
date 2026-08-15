#ifndef FLOYD_WARSHALL_ALGORITHM_H
#define FLOYD_WARSHALL_ALGORITHM_H

#include "interface_graph.h"
#include "route_metric.h"

#include <vector>

using namespace std;

class floyd_warshall_algorithm
{
public:
    floyd_warshall_algorithm(interface_graph &graph_ref);

    double get_shortest_path(
        int start_id,
        int target_id,
        route_metric metric
    );

    void recompute();

    const vector<vector<double>> &get_distance_matrix() const;
    const vector<vector<double>> &get_time_matrix() const;

private:
    interface_graph &graph;

    vector<vector<double>> distance_matrix;
    vector<vector<double>> time_matrix;

    void initialize_matrices();
    void calculate(route_metric metric);
};

#endif