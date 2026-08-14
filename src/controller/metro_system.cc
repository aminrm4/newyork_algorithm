#include "metro_system.h"
#include <chrono>
using namespace std::chrono;

metro_system::metro_system()
    : network(), network_data(),
      bfs(network), dfs(network), dijkstra(network),
      kruskal(network), prim(network),
      express_path(express_network),
      bellman_ford(incentive_network)
{
    network_data.build_network(network);
    build_express_network();
    build_incentive_network(false);
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

void metro_system::build_incentive_network(bool create_negative_cycle){
    int station_count = network.get_station_count();

    incentive_network.resize(station_count);

    for (int station_id = 0; station_id < station_count; station_id++){
        for (auto& e : network.get_neighbors(station_id))
            incentive_network.add_edge(station_id, e.get_destination(), e.get_time()); 
    }

    auto add_incentive =[this](const string& from_name, const string& to_name, double bonus){
        int from = network.find_station_id(from_name);
        int to = network.find_station_id(to_name);

        if (from == -1 || to == -1)
            return;

        incentive_network.add_edge(from, to,-bonus);
    };

    add_incentive("niroogah", "sadeghiyeh", 1.0);
    add_incentive("chehel_derakht", "aminabad", 1.0);

    // Negative Cycle
    if (create_negative_cycle){
        int niroogah = network.find_station_id("niroogah");
        int sadeghiyeh = network.find_station_id("sadeghiyeh");
        int chehel_derakht = network.find_station_id("chehel_derakht");

        if (niroogah != -1 && sadeghiyeh != -1 && chehel_derakht != -1){
            incentive_network.add_edge( niroogah, sadeghiyeh, -10.0);
            incentive_network.add_edge(sadeghiyeh, chehel_derakht, 1.0);
            incentive_network.add_edge(chehel_derakht, niroogah, 1.0);
        }
    }
}

BellmanFordResult metro_system::demo_negative_cycle_detection(){
    build_incentive_network(true);

    int start_id = network.find_station_id("niroogah");

    BellmanFordResult result;

    if (start_id != -1)
        result = bellman_ford.find_shortest_path(start_id, start_id);
    

    build_incentive_network(false);
    return result;
}
BellmanFordResult metro_system::find_incentive_aware_path(int start_id, int target_id){
    return bellman_ford.find_shortest_path(start_id, target_id);
}

std::vector<train> metro_system::schedule_trains(const std::vector<train>& trains){
    return platform.schedule(trains);
}

void metro_system::enqueue_train(const train& t){
    train_dispatch_queue.enqueue_train(t);
}

train metro_system::dispatch_next_train(){
    return train_dispatch_queue.dispatch_next();
}

train metro_system::peek_next_train() const{
    return train_dispatch_queue.peek_next();
}

bool metro_system::dispatch_queue_empty() const{
    return train_dispatch_queue.empty();
}

size_t metro_system::dispatch_queue_size() const{
    return train_dispatch_queue.size();
}