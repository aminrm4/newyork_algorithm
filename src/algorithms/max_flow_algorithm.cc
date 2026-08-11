#include "max_flow_algorithm.h"
#include <algorithm>
#include <limits>
#include <queue>

max_flow_algorithm::max_flow_algorithm(interface_graph& graph_ref, double default_capacity)
    : graph(graph_ref) {

    int station_count = graph.get_station_count();

    capacity_matrix.assign(station_count, vector<double>(station_count, 0.0));

    residual_capacity.assign(station_count, vector<double>(station_count, 0.0));

    build_capacity_matrix(default_capacity);
    reset_residual_capacity();
}

void max_flow_algorithm::build_capacity_matrix(double default_capacity){
    int station_count = graph.get_station_count();

    for (int i = 0; i < station_count; i++){
        vector<edge>& neighbors = graph.get_neighbors(i);

        for (edge& current_edge : neighbors){
            int v = current_edge.get_destination();
            capacity_matrix[i][v] = default_capacity;
        }
    }
}

void max_flow_algorithm::set_capacity(int from_id, int to_id, double capacity){
    if (!graph.has_station(from_id) || !graph.has_station(to_id))
        throw out_of_range("invalid station_id");
    
    if (capacity < 0.0)
        throw invalid_argument("capacity cannot be negative");
    
    capacity_matrix[from_id][to_id] = capacity;
    capacity_matrix[to_id][from_id] = capacity;
}

void max_flow_algorithm::reset_residual_capacity(){
    residual_capacity = capacity_matrix;
}

// TODO: replace temporary bfs with teammates bfs 
bool max_flow_algorithm::bfs(int source_id, int sink_id, vector<int>& parent){
    int station_count = graph.get_station_count();

    parent.assign(station_count, -1);

    vector<bool> visited(station_count, false);

    queue<int> stations_queue;

    stations_queue.push(source_id);
    visited[source_id] = true;

    while (!stations_queue.empty()){
        int current = stations_queue.front();
        stations_queue.pop();

        for (int next = 0; next < station_count; next++){
            if (!visited[next] && residual_capacity[current][next] > 0.0){
                parent[next] = current;
                visited[next] = true;

                if (next == sink_id)
                    return true;
                
                stations_queue.push(next);
            }
        }
    }
    return false;
}

// Endmonds-Karp 
max_flow_result max_flow_algorithm::find_max_flow(int source_id, int sink_id){
    if (!graph.has_station(source_id) || !graph.has_station(sink_id))
        throw out_of_range("invalid station_id");

    if (source_id == sink_id)
        return {0.0};
    

    reset_residual_capacity();

    int station_count = graph.get_station_count();

    vector<int> parent(station_count, -1);

    double total_flow = 0.0;

    while (bfs(source_id, sink_id, parent)){
        double path_flow = numeric_limits<double>::infinity();
        int current = sink_id;

        while (current != source_id){
            int previous = parent[current];

            path_flow = min(path_flow,residual_capacity[previous][current]);

            current = previous;
        }

        current = sink_id;

        while (current != source_id){
            int previous = parent[current];

            residual_capacity[previous][current] -= path_flow;

            residual_capacity[current][previous] += path_flow;

            current = previous;
        }
        total_flow += path_flow;
    }
    return {total_flow};
}