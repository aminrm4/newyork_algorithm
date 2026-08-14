#include "metro_system.h"
#include <chrono>
using namespace std::chrono;

metro_system::metro_system()
    : network(), network_data(),
      bfs(network), dfs(network), dijkstra(network),
      kruskal(network), prim(network),
      express_path(express_network)
{
    network_data.build_network(network);
    build_express_network();
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

void metro_system::build_express_network(){
    express_network.resize(network.get_station_count());

    // Selected stations for the one-way express line
    vector<string> express_route = {
        "terminal_mosaferbari_qom",
        "ghaleh_kamkar",
        "meydan_keshavarz",
        "meydan_motahari",
        "harame_motahar_masoumeh",
        "boostan_fadak"
    };

    for (size_t i = 0; i+1 < express_route.size(); i++){
        int from = network.find_station_id(express_route[i]);
        int to = network.find_station_id(express_route[i + 1]);

        if (from == -1 || to == -1)
            continue;

        for (auto& e : network.get_neighbors(from)){
            if (e.get_destination() == to){
                express_network.add_edge(from, to, e.get_time());
                break;
            }
        }
    }
}

PathResult metro_system::find_express_path(int start_id, int target_id){
    return express_path.find_shortest_path(start_id, target_id);
}