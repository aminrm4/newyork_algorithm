#pragma once

#include "graph.h"
#include "qom_network_data.h"
#include "bfs_algorithm.h"
#include "dfs_algorithm.h"
#include "dijkstra_algorithm.h"
#include "a_star_algorithm.h"

#include "kruskal_algorithm.h"
#include "prim_algorithm.h"
#include "weighted_digraph.h"
#include "dag_shortest_path.h"
#include "bellman_ford_algorithm.h"

#include "platform_scheduler.h"
#include "train.h"
#include "dispatch_queue.h"
#include "network_analytics.h"
#include "passenger_simulator.h"

class metro_system
{
private:
    graph network;
    qom_network_data network_data;
    bfs_algorithm bfs;
    dfs_algorithm dfs;
    dijkstra_algorithm dijkstra;
    a_star_algorithm a_star;

    kruskal_algorithm kruskal;
    prim_algorithm prim;
    weighted_digraph express_network;
    dag_shortest_path_algorithm express_path;

    weighted_digraph incentive_network;
    bellman_ford_algorithm bellman_ford;

    platform_scheduler platform;
    dispatch_queue train_dispatch_queue;

    network_analytics analytics;
    passenger_simulator passenger_sim;

    void build_express_network();
    void build_incentive_network(bool create_negative_cycle = false);

public:
    metro_system();
    PathResult find_a_star_path(int start_id, int target_id, route_metric metric);

    // Round 1
    PathResult find_path(int start_id, int target_id);
    PathResult find_shortest_path(int start_id, int target_id, route_metric metric);
    int get_station_count();
    int get_route_count();
    std::string get_station_name(int station_id);

    // Round 2
    MSTComparisonResult compare_mst_algorithms(route_metric metric);
    PathResult find_express_path(int start_id, int target_id);
    BellmanFordResult find_incentive_aware_path(int start_id, int target_id);
    // test negative cycle
    BellmanFordResult demo_negative_cycle_detection();

    // Round 3
    std::vector<train> schedule_trains(const std::vector<train> &trains);
    void enqueue_train(const train &t);
    train dispatch_next_train();
    train peek_next_train() const;
    bool dispatch_queue_empty() const;
    size_t dispatch_queue_size() const;

    void record_trip(int station_id);
    void finish_day();
    double get_avg_daily_trips();
    int get_kth_busiest_station(int);

    void configure_gate(int gate_capacity);
    void simulate_passenger_arrivals(int current_time, int max_random_passengers);
    void process_passenger_gate(int current_time);
    double get_avg_passenger_waiting_time() const;
    int get_gate_queue_size() const;
    int get_processed_passenger_count() const;
};