#include "articulation_points_finder.h"

articulation_points_finder::articulation_points_finder(interface_graph& graph_ref) : graph(graph_ref), timer(0){}

// TODO: use teammate dfs 
void articulation_points_finder::dfs(int current_id){
    visited[current_id] = true;
    discovery_time[current_id] = timer;
    low_link[current_id] = timer;

    timer++;

    int child_count = 0;

    for (auto& edge : graph.get_neighbors(current_id)){
        int neighbor_id = edge.get_destination();

        // Tree edge
        if (!visited[neighbor_id]){
            parent[neighbor_id] = current_id;
            child_count++;

            dfs(neighbor_id);

            // Update low-link value
            low_link[current_id] = std::min(low_link[current_id], low_link[neighbor_id]);

            // Bridge condition
            if (low_link[neighbor_id] > discovery_time[current_id])
                bridges.emplace_back(current_id, neighbor_id);
            
            // Articulation point condition for DFS root
            if (parent[current_id] == -1 && child_count > 1)
                is_articulation_point[current_id] = true;
            
            // Articulation point condition for non-root
            if (parent[current_id] != -1 && low_link[neighbor_id] >= discovery_time[current_id])
                is_articulation_point[current_id] = true;
            
        }

        // Back edge
        else if (neighbor_id != parent[current_id])
            low_link[current_id] = std::min(low_link[current_id], discovery_time[neighbor_id]);
        
    }
}

articulation_result articulation_points_finder::find(){
    int station_count = graph.get_station_count();

    discovery_time.assign(station_count,-1);
    low_link.assign(station_count,-1);
    parent.assign(station_count,-1);
    visited.assign(station_count, false);
    is_articulation_point.assign(station_count, false);

    bridges.clear();
    timer = 0;

    // The graph may contain multiple connected components.
    for (int station_id = 0; station_id < station_count; station_id++){
        if (!visited[station_id])
            dfs(station_id);        
    }

    articulation_result result;

    for (int station_id = 0; station_id < station_count; station_id++){
        if (is_articulation_point[station_id])
            result.articulation_points.push_back(station_id);
    }

    result.bridges = bridges;
    return result;
}