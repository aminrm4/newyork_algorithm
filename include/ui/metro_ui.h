#pragma once
#include "metro_system.h"

class metro_ui{
    private:
        metro_system& system;

        // Round 1
        void show_network_info();
        void show_accessibility();
        void show_shortest_path();
        void show_path(const PathResult& result);

        // Round 2
        void show_cheapest_network();
    public:
        explicit metro_ui(metro_system& system_ref);
        void run();
};