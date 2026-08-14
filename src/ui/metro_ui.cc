#include "ui/metro_ui.h"
#include "metro_ui.h"

metro_ui::metro_ui(metro_system& system_ref): system(system_ref){}

void metro_ui::run(){
    show_network_info();
    cout << '\n';
    show_accessibility();
    cout << '\n';
    show_shortest_path();
    cout << '\n';
    show_cheapest_network();
    cout << '\n';
    show_express_path();
}

void metro_ui::show_network_info(){
    cout << "Qom Metro Network\n";
    cout << "-----------------\n";

    cout << "Stations: " << system.get_station_count() << '\n';
    cout << "Routes: " << system.get_route_count() << '\n';
}

void metro_ui::show_accessibility(){
    cout << "--- Accessibility ---\n";

    int start_id;
    int target_id;

    cout << "Start station ID: ";
    cin >> start_id;

    cout << "Target station ID: ";
    cin >> target_id;

    try{
        PathResult result = system.find_path(start_id, target_id);

        if (!result.reach_able){
            cout << "No path exists.\n";
            return;
        }
        show_path(result);
    }
    catch (const exception&){
        cerr << "Invalid station ID.\nValid IDs: 0 - " << system.get_station_count()-1 << '\n';
    }
}

void metro_ui::show_shortest_path(){
    cout << "--- Shortest Path ---\n";

    int start_id;
    int target_id;
    int metric_choice;

    cout << "Start station ID: ";
    cin >> start_id;

    cout << "Target station ID: ";
    cin >> target_id;

    cout << "Metric:\n";
    cout << "1. Distance\n";
    cout << "2. Time\n";
    cout << "Choice: ";

    cin >> metric_choice;

    route_metric metric;

    if (metric_choice == 1)
        metric = route_metric::DISTANCE;
    
    else if (metric_choice == 2)
        metric = route_metric::TIME;
    else{
        cout << "Invalid metric.\n";
        return;
    }

    try{
        PathResult result = system.find_shortest_path(start_id, target_id, metric);

        if (!result.reach_able){
            cout << "No path exists.\n";
            return;
        }

        show_path(result);
        cout << "Total cost: " << result.total_cost << '\n';
    }
    catch (const exception&){
        cerr << "Invalid station ID.\nValid IDs: 0 - " << system.get_station_count()-1 << '\n';
    }
}

void metro_ui::show_path(const PathResult& result){
    cout << "Path: ";

    for (size_t i = 0; i < result.path.size(); i++){
        int station_id = result.path[i];

        cout << system.get_station_name(station_id);
        if (i + 1 < result.path.size())
            cout << " -> ";
    }
    cout << '\n';
}

void metro_ui::show_cheapest_network(){
    cout << "--- T2.1: Minimum Cost Network (MST) ---\n";
 
    int metric_choice;
    cout << "Edge weight metric:\n1. Distance\n2. Time\nChoice: ";
    cin >> metric_choice;
 
    route_metric metric;
    switch (metric_choice){
        case 1:
            metric = route_metric::DISTANCE;
            break;
        case 2:
            metric = route_metric::TIME;
            break;
        default:
            cout << "Invalid metric.\n";
            return;
    }

    MSTComparisonResult comparison = system.compare_mst_algorithms(metric);
 
    cout << "\n[Kruskal] Total cost: " << comparison.kruskal_result.total_weight
         << " | Number of edges: " << comparison.kruskal_result.edges.size()
         << " | Execution time: " << comparison.kruskal_time_ms << " ms\n";
 
    cout << "[Prim]    Total cost: " << comparison.prim_result.total_weight
         << " | Number of edges: " << comparison.prim_result.edges.size()
         << " | Execution time: " << comparison.prim_time_ms << " ms\n";
 
    if (!comparison.kruskal_result.is_spanning_tree || !comparison.prim_result.is_spanning_tree)
        cout << "Note: The graph is not connected; a complete spanning tree could not be created.\n";
 
    cout << "\nSelected edges (Kruskal):\n";
    for (auto& e : comparison.kruskal_result.edges){
        cout << "  " << system.get_station_name(e.from) << " -- " << system.get_station_name(e.to) << "  (" << e.weight << ")\n";
    }
}
void metro_ui::show_express_path(){
    cout << "--- T2.3: One-Way Express Line (DAG) ---\n";

    int start_id, target_id;

    cout << "Start station ID: ";
    cin >> start_id;

    cout << "Target station ID: ";
    cin >> target_id;

    try {
        PathResult result = system.find_express_path(start_id,target_id);

        if (!result.reach_able){
            cout << "No path exists on the express line.\n";
            return;
        }

        show_path(result);
        cout << "Total time: " << result.total_cost << " minutes\n";
    }
    catch (const out_of_range&){
        cerr << "Invalid station ID.\n" << "Valid IDs: 0 - " << system.get_station_count()-1 << '\n';
    }
    catch (const logic_error& e){
        cerr << "Express line error: " << e.what() << '\n';
    }
}