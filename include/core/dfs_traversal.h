#pragma once
#include <functional>
#include <vector>

using namespace std;

class dfs_traversal{
    public:
        using neighbor_provider = function<vector<int>(int)>;

        using enter_callback = function<void(int)>;
        using tree_edge_callback = function<void(int, int)>;
        using back_edge_callback = function<void(int, int)>;
        using exit_callback = function<void(int)>;

        void traverse(
            int start_id,
            vector<bool>& visited,
            vector<int>& parent,
            neighbor_provider get_neighbors,
            enter_callback on_enter,
            tree_edge_callback on_tree_edge,
            back_edge_callback on_back_edge,
            exit_callback on_exit
        );

    private:
        void visit(
            int current_id,
            vector<bool>& visited,
            vector<int>& parent,
            neighbor_provider& get_neighbors,
            enter_callback& on_enter,
            tree_edge_callback& on_tree_edge,
            back_edge_callback& on_back_edge,
            exit_callback& on_exit
        );
};
