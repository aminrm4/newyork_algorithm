#include "floyd_warshall_algorithm.h"

#include <limits>

floyd_warshall_algorithm::floyd_warshall_algorithm(
    interface_graph &graph_ref)
    : graph(graph_ref)
{
    initialize_matrices();

    calculate(route_metric::DISTANCE);
    calculate(route_metric::TIME);
}

void floyd_warshall_algorithm::initialize_matrices()
{
    int station_count = graph.get_station_count();

    double infinity = numeric_limits<double>::infinity();

    distance_matrix.assign(
        station_count,
        vector<double>(station_count, infinity));

    time_matrix.assign(
        station_count,
        vector<double>(station_count, infinity));

    for (int i = 0; i < station_count; ++i)
    {
        distance_matrix[i][i] = 0.0;
        time_matrix[i][i] = 0.0;

        vector<edge> &neighbors = graph.get_neighbors(i);

        for (edge &e : neighbors)
        {
            int destination = e.get_destination();

            if (e.get_distance() < distance_matrix[i][destination])
            {
                distance_matrix[i][destination] = e.get_distance();
            }

            if (e.get_time() < time_matrix[i][destination])
            {
                time_matrix[i][destination] = e.get_time();
            }
        }
    }
}

void floyd_warshall_algorithm::calculate(route_metric metric)
{
    int station_count = graph.get_station_count();

    vector<vector<double>> &matrix =
        metric == route_metric::DISTANCE
            ? distance_matrix
            : time_matrix;

    double infinity = numeric_limits<double>::infinity();

    for (int k = 0; k < station_count; ++k)
    {
        for (int i = 0; i < station_count; ++i)
        {
            if (matrix[i][k] == infinity)
            {
                continue;
            }

            for (int j = 0; j < station_count; ++j)
            {
                if (matrix[k][j] == infinity)
                {
                    continue;
                }

                double new_cost =
                    matrix[i][k] + matrix[k][j];

                if (new_cost < matrix[i][j])
                {
                    matrix[i][j] = new_cost;
                }
            }
        }
    }
}

double floyd_warshall_algorithm::get_shortest_path(
    int start_id,
    int target_id,
    route_metric metric)
{
    if (metric == route_metric::DISTANCE)
    {
        return distance_matrix[start_id][target_id];
    }

    return time_matrix[start_id][target_id];
}

const vector<vector<double>> &
floyd_warshall_algorithm::get_distance_matrix() const
{
    return distance_matrix;
}

const vector<vector<double>> &
floyd_warshall_algorithm::get_time_matrix() const
{
    return time_matrix;
}