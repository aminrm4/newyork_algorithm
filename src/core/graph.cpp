#include "graph.h"
#include <iostream>

graph::graph()
    : route_count(0)
{
}

int graph::add_station(const string &station_name)
{
    auto existing = name_to_id.find(station_name);
    if (existing != name_to_id.end())
    {
        return existing->second;
    }

    int new_id = static_cast<int>(stations.size());
    stations.emplace_back(new_id, station_name);
    adjacency_list.emplace_back();
    name_to_id[station_name] = new_id;

    return new_id;
}

int graph::add_station(const string &station_name, double latitude, double longitude)
{
    auto existing = name_to_id.find(station_name);
    if (existing != name_to_id.end())
    {
        return existing->second;
    }

    int new_id = static_cast<int>(stations.size());
    stations.emplace_back(new_id, station_name, latitude, longitude);
    adjacency_list.emplace_back();
    name_to_id[station_name] = new_id;

    return new_id;
}

void graph::add_route(int from_id, int to_id, double distance_km, double time_min)
{
    validate_station_id(from_id);
    validate_station_id(to_id);

    adjacency_list[from_id].emplace_back(to_id, distance_km, time_min);
    route_count++;

    adjacency_list[to_id].emplace_back(from_id, distance_km, time_min);
}

int graph::get_station_count()
{
    return static_cast<int>(stations.size());
}

vector<edge> &graph::get_neighbors(int station_id)
{
    validate_station_id(station_id);
    return adjacency_list[station_id];
}

bool graph::has_station(int station_id)
{
    return station_id >= 0 && station_id < static_cast<int>(stations.size());
}

int graph::find_station_id(const string &station_name)
{
    auto found = name_to_id.find(station_name);
    if (found == name_to_id.end())
    {
        return -1;
    }
    return found->second;
}

station &graph::get_station(int station_id)
{
    validate_station_id(station_id);
    return stations[station_id];
}

vector<station> &graph::get_all_stations()
{
    return stations;
}

int graph::get_route_count()
{
    return route_count;
}
// write only for test , can be deleted
void graph::print_graph()
{
    cout << "number of stations " << get_station_count() << "\n";
    cout << "number 0f routes " << get_route_count() << "\n\n";

    for (auto &s : stations)
    {
        cout << "[" << s.get_id() << "] " << s.get_name() << "\n";
        for (auto &e : adjacency_list[s.get_id()])
        {
            station &dest = stations[e.get_destination()];
            cout << "    -> " << dest.get_name()
                 << " | distance: " << e.get_distance() << " km"
                 << " | time: " << e.get_time() << " min\n";
        }
    }
}

void graph::validate_station_id(int station_id)
{
    if (!has_station(station_id))
    {
        throw out_of_range("invalid station_id");
    }
}