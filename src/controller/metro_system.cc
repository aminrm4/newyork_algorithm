#include "metro_system.h"
#include <chrono>
using namespace std::chrono;

metro_system::metro_system()
    : network(),  network_data(),
      bfs(network), dfs(network),
      dijkstra(network), a_star(network),
      kruskal(network), prim(network),
      express_path(express_network),
      bellman_ford(incentive_network),
      passenger_sim(1), max_flow(network, 0.0) , floyd_warshall(network)
      , articulation_finder(network), dominating_set(network)
      , levenshtein(network)
{
    network_data.build_network(network);
    build_express_network();
    build_incentive_network(false);
    floyd_warshall.recompute();
    max_flow.initialize(0.0);
}

PathResult metro_system::find_path(int start_id, int target_id)
{
    return bfs.find_path(start_id, target_id);
}

PathResult metro_system::find_shortest_path(int start_id, int target_id, route_metric metric)
{
    return dijkstra.find_shortest_path(start_id, target_id, metric);
}

PathResult metro_system::find_a_star_path(int start_id, int target_id, route_metric metric)
{
    return a_star.find_shortest_path(start_id, target_id, metric);
}

int metro_system::get_station_count()
{
    return network.get_station_count();
}

int metro_system::get_route_count()
{
    return network.get_route_count();
}

std::string metro_system::get_station_name(int station_id)
{
    return network.get_station(station_id).get_name();
}

MSTComparisonResult metro_system::compare_mst_algorithms(route_metric metric)
{
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

void metro_system::build_express_network()
{
    express_network.resize(network.get_station_count());

    // Selected stations for the one-way express line
    vector<string> express_route = {
        "terminal_mosaferbari_qom",
        "ghaleh_kamkar",
        "meydan_keshavarz",
        "meydan_motahari",
        "harame_motahar_masoumeh",
        "boostan_fadak"};

    for (size_t i = 0; i + 1 < express_route.size(); i++)
    {
        int from = network.find_station_id(express_route[i]);
        int to = network.find_station_id(express_route[i + 1]);

        if (from == -1 || to == -1)
            continue;

        for (auto &e : network.get_neighbors(from))
        {
            if (e.get_destination() == to)
            {
                express_network.add_edge(from, to, e.get_time());
                break;
            }
        }
    }
}

PathResult metro_system::find_express_path(int start_id, int target_id)
{
    return express_path.find_shortest_path(start_id, target_id);
}

void metro_system::build_incentive_network(bool create_negative_cycle)
{
    int station_count = network.get_station_count();

    incentive_network.resize(station_count);

    for (int station_id = 0; station_id < station_count; station_id++)
    {
        for (auto &e : network.get_neighbors(station_id))
            incentive_network.add_edge(station_id, e.get_destination(), e.get_time());
    }

    auto add_incentive = [this](const string &from_name, const string &to_name, double bonus)
    {
        int from = network.find_station_id(from_name);
        int to = network.find_station_id(to_name);

        if (from == -1 || to == -1)
            return;

        incentive_network.add_edge(from, to, -bonus);
    };

    add_incentive("niroogah", "sadeghiyeh", 1.0);
    add_incentive("chehel_derakht", "aminabad", 1.0);

    // Negative Cycle
    if (create_negative_cycle)
    {
        int niroogah = network.find_station_id("niroogah");
        int sadeghiyeh = network.find_station_id("sadeghiyeh");
        int chehel_derakht = network.find_station_id("chehel_derakht");

        if (niroogah != -1 && sadeghiyeh != -1 && chehel_derakht != -1)
        {
            incentive_network.add_edge(niroogah, sadeghiyeh, -10.0);
            incentive_network.add_edge(sadeghiyeh, chehel_derakht, 1.0);
            incentive_network.add_edge(chehel_derakht, niroogah, 1.0);
        }
    }
}

BellmanFordResult metro_system::demo_negative_cycle_detection()
{
    build_incentive_network(true);

    int start_id = network.find_station_id("niroogah");

    BellmanFordResult result;

    if (start_id != -1)
        result = bellman_ford.find_shortest_path(start_id, start_id);

    build_incentive_network(false);
    return result;
}
BellmanFordResult metro_system::find_incentive_aware_path(int start_id, int target_id)
{
    return bellman_ford.find_shortest_path(start_id, target_id);
}

std::vector<train> metro_system::schedule_trains(const std::vector<train> &trains)
{
    return platform.schedule(trains);
}

void metro_system::enqueue_train(const train &t)
{
    train_dispatch_queue.enqueue_train(t);
}

train metro_system::dispatch_next_train()
{
    return train_dispatch_queue.dispatch_next();
}

train metro_system::peek_next_train() const
{
    return train_dispatch_queue.peek_next();
}

bool metro_system::dispatch_queue_empty() const
{
    return train_dispatch_queue.empty();
}

size_t metro_system::dispatch_queue_size() const
{
    return train_dispatch_queue.size();
}
void metro_system::record_trip(int station_id)
{
    if (!network.has_station(station_id))

        throw std::out_of_range("invalid station_id");

    analytics.record_trip(station_id);
}

void metro_system::finish_day()
{
    analytics.finish_day();
}

double metro_system::get_avg_daily_trips()
{
    return analytics.average_daily_trips();
}

int metro_system::get_kth_busiest_station(int k)
{
    return analytics.kth_busiest_station(k);
}

void metro_system::configure_gate(int gate_capacity)
{
    passenger_sim = passenger_simulator(gate_capacity);
}

void metro_system::simulate_passenger_arrivals(int current_time, int max_random_passengers)
{
    passenger_sim.generate_random_passengers(max_random_passengers, current_time);
}

void metro_system::process_passenger_gate(int current_time)
{
    passenger_sim.process_gate(current_time);
}

double metro_system::get_avg_passenger_waiting_time() const
{
    return passenger_sim.average_waiting_time();
}

int metro_system::get_gate_queue_size() const
{
    return passenger_sim.queue_size();
}

int metro_system::get_processed_passenger_count() const
{
    return passenger_sim.processed_count();
}

void metro_system::set_route_capacity(int from_id, int to_id, double capacity){
    max_flow.set_capacity(from_id, to_id, capacity);
}

max_flow_result metro_system::find_max_passengers(int source_id, int target_id){
    return max_flow.find_max_flow(source_id, target_id);
}

double metro_system::get_all_pairs_shortest_path(int start_id, int target_id, route_metric metric){
    if (start_id < 0 || start_id >= network.get_station_count() || 
        target_id < 0 || target_id >= network.get_station_count()){
        throw out_of_range("invalid station_id");
    }

    return floyd_warshall.get_shortest_path(start_id, target_id, metric );
}
const vector<vector<double>>& metro_system::get_distance_matrix() const{
    return floyd_warshall.get_distance_matrix();
}

const vector<vector<double>>& metro_system::get_time_matrix() const{
    return floyd_warshall.get_time_matrix();
}
articulation_result metro_system::find_critical_stations(){
    return articulation_finder.find();
}
vector<int> metro_system::find_emergency_team_stations(){
    return dominating_set.find_dominating_set();
}

station metro_system::find_closest_station(const std::string& name){
    return levenshtein.find_closest(name);
}