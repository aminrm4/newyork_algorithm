#include "controller/metro_system.h"

metro_system::metro_system(): network(), network_data(), bfs(network), dfs(network), dijkstra(network){
    network_data.build_network(network);
}

PathResult metro_system::find_path(int start_id, int target_id){
    return bfs.find_path(start_id, target_id);
}

PathResult metro_system::find_shortest_path(int start_id, int target_id, route_metric metric){
    return dijkstra.find_shortest_path(start_id, target_id, metric);
}

int metro_system::get_station_count(){
    return network.get_station_count();
}

int metro_system::get_route_count(){
    return network.get_route_count();
}

std::string metro_system::get_station_name(int station_id){
    return network.get_station(station_id).get_name();
}