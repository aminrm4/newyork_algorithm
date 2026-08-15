#pragma once
#include "metro_system.h"

class metro_ui{
    private:
        metro_system& system;

        // Round 1
        void show_network_info();
        void show_accessibility();
        void show_shortest_path();
        void show_a_star_path();
        void show_path(const PathResult& result);

        // Round 2
        void show_cheapest_network();
        void show_express_path();
        void show_incentive_aware_path();
        void show_negative_cycle_test();

        // Round 3
        void show_platform_scheduling();
        void show_train_dispatch_queue();
        void show_network_analytics();
        void show_passenger_simulation();

        // Round 4
        void show_all_pairs_shortest_path();
        void show_max_flow();
    public:
        explicit metro_ui(metro_system& system_ref);
        void run();
};