#include "bfs_traversal.h"
#include <queue>

bool bfs_traversal::search(int start_id, int target_id, int node_count, vector<int>& parent, neighbor_provider get_neighbors){
    parent.assign(node_count, -1);
    vector<bool> visited(node_count, false);
    queue<int> pending;

    visited[start_id] = true;
    pending.push(start_id);

    while (!pending.empty()){
        int current = pending.front();
        pending.pop();

        if (current == target_id)
            return true;

        vector<int> neighbors = get_neighbors(current);

        for (int neighbor : neighbors){
            if (!visited[neighbor]){
                visited[neighbor] = true;
                parent[neighbor] = current;
                pending.push(neighbor);
            }
        }
    }
    return false;
}