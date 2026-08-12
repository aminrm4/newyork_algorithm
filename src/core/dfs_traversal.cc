#include "dfs_traversal.h"

void dfs_traversal::traverse(
    int start_id,
    vector<bool>& visited,
    vector<int>& parent,
    neighbor_provider get_neighbors,
    enter_callback on_enter,
    tree_edge_callback on_tree_edge,
    back_edge_callback on_back_edge,
    exit_callback on_exit){
    visit(
        start_id,
        visited,
        parent,
        get_neighbors,
        on_enter,
        on_tree_edge,
        on_back_edge,
        on_exit
    );
}

void dfs_traversal::visit(
    int current_id,
    vector<bool>& visited,
    vector<int>& parent,
    neighbor_provider& get_neighbors,
    enter_callback& on_enter,
    tree_edge_callback& on_tree_edge,
    back_edge_callback& on_back_edge,
    exit_callback& on_exit){
    visited[current_id] = true;

    if (on_enter)
        on_enter(current_id);

    for (int neighbor_id : get_neighbors(current_id)){
        if (!visited[neighbor_id]){
            parent[neighbor_id] = current_id;

            if (on_tree_edge)
                on_tree_edge(current_id, neighbor_id);

            visit(neighbor_id, visited, parent, get_neighbors, on_enter, on_tree_edge, on_back_edge, on_exit);
        }
        else if (neighbor_id != parent[current_id]){
            if (on_back_edge)
                on_back_edge(current_id, neighbor_id);
        }
    }

    if (on_exit)
        on_exit(current_id);
}