#pragma once
#include <vector>
#include <functional>
using namespace std;

class bfs_traversal{
    public:
        using neighbor_provider = function<vector<int>(int)>;
        bool search(int start_id, int target_id, int node_count, vector<int>& parent, neighbor_provider get_neighbors);
};