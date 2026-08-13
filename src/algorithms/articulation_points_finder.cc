#include "articulation_points_finder.h"
#include "dfs_traversal.h"

#include <algorithm>

articulation_points_finder::articulation_points_finder(interface_graph& graph_ref)
    : graph(graph_ref), timer(0) {}

articulation_result articulation_points_finder::find(){
    int station_count = graph.get_station_count();

    discovery_time.assign(station_count, -1);
    low_link.assign(station_count, -1);
    parent.assign(station_count, -1);
    visited.assign(station_count, false);
    is_articulation_point.assign(station_count, false);

    bridges.clear();
    timer = 0;

    dfs_traversal traversal;

    auto get_neighbors = [this](int current_id) -> vector<int>{
        vector<int> neighbors;

        for (auto& e : graph.get_neighbors(current_id))
            neighbors.push_back(e.get_destination());
        
        return neighbors;
    };

    for (int station_id = 0; station_id < station_count; station_id++){
        if (visited[station_id])
            continue;

        int root = station_id;
        int child_count = 0;

        auto on_enter = [this](int current_id){
            discovery_time[current_id] = timer;
            low_link[current_id] = timer;
            timer++;
        };

        auto on_tree_edge = [this,root, &child_count](int current_id, int neighbor_id){
            parent[neighbor_id] = current_id;

            if(current_id == root)
            child_count++;
        };

        auto on_back_edge = [this](int current_id, int neighbor_id){
            if (discovery_time[neighbor_id] < discovery_time[current_id])
                low_link[current_id] = min(low_link[current_id], discovery_time[neighbor_id]);
        };

        auto on_exit = [this, root](int current_id){
            if (parent[current_id] == -1)
                return;

            int p = parent[current_id];

            // Update low-link value of parent
            low_link[p] = min(low_link[p], low_link[current_id]);

            // Bridge condition
            if (low_link[current_id] > discovery_time[p])
                bridges.emplace_back(p, current_id);
            

            // Articulation point condition for non-root
            if (p != root && low_link[current_id] >= discovery_time[p])
                is_articulation_point[p] = true;     
        };

        traversal.traverse(station_id, visited, parent, get_neighbors, on_enter, on_tree_edge, on_back_edge, on_exit);

        // Articulation point condition for DFS root
        if (child_count > 1)
            is_articulation_point[root] = true;   
    }
    articulation_result result;

    for (int station_id = 0; station_id < station_count; station_id++){
        if (is_articulation_point[station_id])
            result.articulation_points.push_back(station_id);    
    }

    result.bridges = bridges;
    return result;
}