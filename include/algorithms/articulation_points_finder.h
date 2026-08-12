#pragma once
#include <vector>
#include "interface_graph.h"

struct articulation_result{
    vector<int> articulation_points;
    vector<pair<int, int>> bridges;
};

class articulation_points_finder{
    public:
        articulation_points_finder(interface_graph& graph_ref);
        articulation_result find();

    private:
        interface_graph& graph;
        int timer;
        vector<int> discovery_time;
        vector<int> low_link;
        vector<int> parent;

        vector<bool> visited;
        vector<bool> is_articulation_point;

        vector<pair<int, int>> bridges;
        void dfs(int current_id);
};