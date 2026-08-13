#pragma once
#include "controller/metro_system.h"

class metro_ui{
    private:
        metro_system& system;
        void show_network_info();
        void show_accessibility();
        void show_shortest_path();
        void show_path(const PathResult& result);
    public:
        explicit metro_ui(metro_system& system_ref);
        void run();
};