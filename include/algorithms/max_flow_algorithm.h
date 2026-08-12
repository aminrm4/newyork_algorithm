# pragma once
#include "interface_graph.h"
#include <vector>

struct max_flow_result{
    double total_flow;
};

class max_flow_algorithm {
    public:
        max_flow_algorithm(interface_graph& graph_ref, double default_cap);
        max_flow_result find_max_flow(int source_id, int sink_id);
        void set_capacity(int from_id, int to_id, double capacity);
    private:
        interface_graph& graph;
        vector<vector<double>> capacity_matrix;
        vector<vector<double>> residual_capacity;

        bool bfs(int source_id, int sink_id, vector<int>& parent);
        void build_capacity_matrix(double defualt_capacity);
        void reset_residual_capacity();
};