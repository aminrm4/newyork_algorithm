#include "metro_system.h"
#include <chrono>
using namespace std;
using namespace std::chrono;

metro_system::metro_system()
    : network(), network_data(),
      bfs(network), dfs(network), dijkstra(network),
      kruskal(network), prim(network),
      express_network(), express_path(express_network),
      incentive_network(), bellman_ford(incentive_network)
{
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

MSTComparisonResult metro_system::compare_mst_algorithms(route_metric metric){
    MSTComparisonResult comparison;
 
    auto kruskal_start = high_resolution_clock::now();
    comparison.kruskal_result = kruskal.build_mst(metric);
    auto kruskal_end = high_resolution_clock::now();
    comparison.kruskal_time_ms = duration<double, milli>(kruskal_end - kruskal_start).count();
 
    auto prim_start = high_resolution_clock::now();
    comparison.prim_result = prim.build_mst(metric);
    auto prim_end = high_resolution_clock::now();
    comparison.prim_time_ms = duration<double, milli>(prim_end - prim_start).count();
 
    return comparison;
}