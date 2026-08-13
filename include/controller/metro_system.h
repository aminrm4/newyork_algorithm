#pragma once

#include "graph.h"
#include "qom_network_data.h"
#include "bfs_algorithm.h"
#include "dfs_algorithm.h"
#include "dijkstra_algorithm.h"

class metro_system{
private:
    graph network;
    qom_network_data network_data;

    bfs_algorithm bfs;
    dfs_algorithm dfs;
    dijkstra_algorithm dijkstra;

public:
    metro_system();
    PathResult find_path(int start_id, int target_id);

    PathResult find_shortest_path(int start_id, int target_id, route_metric metric);

    int get_station_count();
    int get_route_count();
    std::string get_station_name(int station_id);
};