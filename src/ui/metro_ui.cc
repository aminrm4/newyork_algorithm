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
    cout << '\n';
    show_incentive_aware_path();
    cout << '\n';
    // show_negative_cycle_test(); 
    cout << '\n';
    show_platform_scheduling();
    cout << '\n';
    show_train_dispatch_queue();
    cout << '\n';
    show_network_analytics();
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

void metro_ui::show_incentive_aware_path(){
    cout << "--- T2.4: Incentive-Aware Path (Bellman-Ford) ---\n";

    int start_id, target_id;

    cout << "Start station ID: ";
    cin >> start_id;

    cout << "Target station ID: ";
    cin >> target_id;

    try{
        BellmanFordResult result = system.find_incentive_aware_path(start_id, target_id );

        if (result.negative_cycle_detected){
            cout << "A negative cycle was detected in the network.\n";
            return;
        }
        if (!result.reach_able){
            cout << "No path exists between these stations.\n";
            return;
        }

        cout << "Path: ";

        for (size_t i = 0; i < result.path.size(); i++){
            cout << system.get_station_name(result.path[i]);

            if (i + 1 < result.path.size())
                cout << " -> ";
        }

        cout << "\nFinal cost (including incentives): " << result.total_cost << '\n';
    }
    catch (const out_of_range&){
        cerr << "Invalid station ID.\nValid IDs: 0 - "  << system.get_station_count()-1 << '\n';
    }
}

void metro_ui::show_negative_cycle_test(){
    cout << "--- Negative Cycle Test ---\n";

    BellmanFordResult result = system.demo_negative_cycle_detection();

    if (result.negative_cycle_detected)
        cout << "Negative cycle detected successfully.\n";
    else
        cout << "Negative cycle was NOT detected.\n";
}

void metro_ui::show_platform_scheduling(){
    cout << "--- T3.1: Maximum Train Scheduling on Shared Platform ---\n";

    int train_count;

    cout << "Number of trains: ";
    cin >> train_count;

    if (train_count < 0){
        cout << "Invalid number of trains.\n";
        return;
    }

    vector<train> trains;

    for (int i = 0; i < train_count; i++){
        int arrival, departure;

        cout << "\nTrain " << i + 1 << ":\n";
        cout << "Arrival time: ";
        cin >> arrival;

        cout << "Departure time: ";
        cin >> departure;

        if (arrival >= departure){
            cout << "Invalid interval: arrival time must be less than departure time.\n";
            return;
        }

        trains.emplace_back(i+1, arrival, departure);
    }

    vector<train> selected_trains = system.schedule_trains(trains);

    cout << "\nSelected trains:\n";

    for (const auto& t : selected_trains)
        cout << "Train " << t.get_id() << " [" << t.get_arrival_time() << ", " << t.get_departure_time() << "]\n";
    
    cout << "Maximum number of trains: " << selected_trains.size() << '\n';
}

void metro_ui::show_train_dispatch_queue(){
    cout << "--- T3.2: Train Dispatch Priority Queue ---\n";

    int train_count;

    cout << "Number of trains to enqueue: ";
    cin >> train_count;

    if (train_count < 0){
        cout << "Invalid number of trains.\n";
        return;
    }

    for (int i = 0; i < train_count; i++){
        int arrival, departure;

        cout << "\nTrain " << i + 1 << ":\n";

        cout << "Arrival time: "; cin >> arrival;
        cout << "Departure time: "; cin >> departure;

        if (arrival >= departure){
            cout << "Invalid interval.\n";
            return;
        }

        train t(i + 1, arrival, departure);
        system.enqueue_train(t);
    }

    cout << "\nNumber of trains in queue: " << system.dispatch_queue_size() << '\n';

    if (system.dispatch_queue_empty()){
        cout << "Dispatch queue is empty.\n";
        return;
    }

    cout << "\nHighest priority train:\n";

    train next = system.peek_next_train();

    cout << "Train " << next.get_id() << " [" << next.get_arrival_time() << ", " << next.get_departure_time() << "]\n";

    cout << "\nDispatch order:\n";

    while (!system.dispatch_queue_empty()){
        train dispatched = system.dispatch_next_train();
        cout << "Train " << dispatched.get_id() << " [" << dispatched.get_arrival_time() << ", " << dispatched.get_departure_time() << "]\n";
    }
}

void metro_ui::show_network_analytics(){
    cout << "--- T3.3: Network Operations Analytics ---\n";

    int trip_count;

    cout << "Number of trips to record: ";
    cin >> trip_count;

    if (trip_count < 0){
        cout << "Invalid number of trips.\n";
        return;
    }

    for (int i = 0; i < trip_count; i++){
        int station_id;

        cout << "Trip " << i+1 << " station ID: ";
        cin >> station_id;

        try{
            system.record_trip(station_id);
        }
        catch (const out_of_range&){
            cout << "Invalid station ID.\n";
            i--;
        }
    }

    int day_count;

    cout << "\nNumber of days completed: ";
    cin >> day_count;

    if (day_count < 0){
        cout << "Invalid number of days.\n";
        return;
    }

    for (int i = 0; i < day_count; i++)
        system.finish_day();
    

    cout << "\nAverage daily trips: " << system.get_avg_daily_trips() << '\n';

    int k;
    cout << "\nEnter k for kth busiest station: ";
    cin >> k;

    try{
        int station_id = system.get_kth_busiest_station(k);

        cout << k << "th busiest station: " << system.get_station_name(station_id) << " (ID: " << station_id << ")\n";
    }
    catch (const out_of_range&){
        cout << "Invalid k.\n";
    }
}