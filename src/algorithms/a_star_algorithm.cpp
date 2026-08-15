#include "a_star_algorithm.h"
#include <queue>
#include <limits>
#include <algorithm>
#include <cmath>
#include <utility>

using namespace std;

a_star_algorithm::a_star_algorithm(interface_graph &graph_ref)
    : graph(graph_ref),
      distance_scale(0.0),
      time_scale(0.0)
{
}

double a_star_algorithm::get_edge_weight(edge &e, route_metric metric)
{
    if (metric == route_metric::DISTANCE)
        return e.get_distance();

    return e.get_time();
}

double a_star_algorithm::haversine_distance(station &from, station &to)
{
    constexpr double earth_radius_km = 6371.0088;
    constexpr double pi = 3.14159265358979323846;

    double lat1 = from.get_latitude() * pi / 180.0;
    double lat2 = to.get_latitude() * pi / 180.0;
    double dlat = (to.get_latitude() - from.get_latitude()) * pi / 180.0;
    double dlon = (to.get_longitude() - from.get_longitude()) * pi / 180.0;

    double a = sin(dlat / 2.0) * sin(dlat / 2.0) +
               cos(lat1) * cos(lat2) *
                   sin(dlon / 2.0) * sin(dlon / 2.0);

    return earth_radius_km * 2.0 * asin(sqrt(a));
}

void a_star_algorithm::calculate_scales()
{
    double min_distance_ratio = numeric_limits<double>::infinity();
    double min_time_ratio = numeric_limits<double>::infinity();

    for (int i = 0; i < graph.get_station_count(); i++)
    {
        station &from = graph.get_station(i);

        for (auto &e : graph.get_neighbors(i))
        {
            station &to = graph.get_station(e.get_destination());
            double geographic_distance = haversine_distance(from, to);

            if (geographic_distance <= 0.0)
                continue;

            min_distance_ratio = min(min_distance_ratio,
                                     e.get_distance() / geographic_distance);
            min_time_ratio = min(min_time_ratio,
                                 e.get_time() / geographic_distance);
        }
    }

    if (isfinite(min_distance_ratio) && min_distance_ratio > 0.0)
        distance_scale = min_distance_ratio;

    if (isfinite(min_time_ratio) && min_time_ratio > 0.0)
        time_scale = min_time_ratio;
}

double a_star_algorithm::heuristic(int station_id, int target_id, route_metric metric)
{
    station &current = graph.get_station(station_id);
    station &target = graph.get_station(target_id);

    double geographic_distance = haversine_distance(current, target);

    if (metric == route_metric::DISTANCE)
        return distance_scale * geographic_distance;

    return time_scale * geographic_distance;
}

PathResult a_star_algorithm::find_shortest_path(int start_id, int target_id, route_metric metric)
{
    calculate_scales();
    PathResult result;

    if (!graph.has_station(start_id) || !graph.has_station(target_id))
        throw out_of_range("invalid station_id");

    int station_count = graph.get_station_count();
    double infinity = numeric_limits<double>::infinity();

    vector<double> best_cost(station_count, infinity);
    vector<int> parent(station_count, -1);
    vector<bool> finalized(station_count, false);

    priority_queue<pair<double, int>,
                   vector<pair<double, int>>,
                   greater<pair<double, int>>>
        pending;

    best_cost[start_id] = 0.0;
    pending.push(make_pair(heuristic(start_id, target_id, metric), start_id));

    while (!pending.empty())
    {
        pair<double, int> top = pending.top();
        pending.pop();

        int current_id = top.second;

        if (finalized[current_id])
            continue;

        finalized[current_id] = true;
        result.expanded_nodes++;

        if (current_id == target_id)
            break;

        for (auto &e : graph.get_neighbors(current_id))
        {
            int neighbor = e.get_destination();
            double new_cost = best_cost[current_id] + get_edge_weight(e, metric);

            if (new_cost < best_cost[neighbor])
            {
                best_cost[neighbor] = new_cost;
                parent[neighbor] = current_id;

                double priority = new_cost + heuristic(neighbor, target_id, metric);
                pending.push(make_pair(priority, neighbor));
            }
        }
    }

    if (best_cost[target_id] == infinity)
        return result;

    vector<int> path;

    for (int at = target_id; at != -1; at = parent[at])
        path.push_back(at);

    reverse(path.begin(), path.end());

    result.reach_able = true;
    result.path = path;
    result.total_cost = best_cost[target_id];

    return result;
}
