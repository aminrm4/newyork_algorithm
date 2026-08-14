#pragma once

#include "graph.h"
#include "qom_network_data.h"
#include "bfs_algorithm.h"
#include "dfs_algorithm.h"
#include "dijkstra_algorithm.h"

#include "kruskal_algorithm.h"
#include "prim_algorithm.h"
#include "weighted_digraph.h"
#include "dag_shortest_path.h"
#include "bellman_ford_algorithm.h"

class metro_system {
    private:
        graph network;
        qom_network_data network_data;

        bfs_algorithm bfs;
        dfs_algorithm dfs;
        dijkstra_algorithm dijkstra;

        kruskal_algorithm kruskal;
        prim_algorithm prim;
        weighted_digraph express_network;
        dag_shortest_path_algorithm express_path;

        void build_express_network();

    public:
        metro_system();

        // Round 1
        PathResult find_path(int start_id, int target_id);
        PathResult find_shortest_path(int start_id, int target_id, route_metric metric);
        int get_station_count();
        int get_route_count();
        std::string get_station_name(int station_id);

        // Round 2
        MSTComparisonResult compare_mst_algorithms(route_metric metric);
        PathResult find_express_path(int start_id, int target_id);
};