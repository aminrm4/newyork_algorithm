#include <iostream>
#include "graph.h"
#include "qom_network_data.h"
// all is test can be deleted
int main()
{
    graph metro_graph;
    qom_network_data network_data;
    network_data.build_network(metro_graph);

    metro_graph.print_graph();

    cout << "\n";
    cout << "station count: " << metro_graph.get_station_count() << "\n";
    cout << "route count: " << metro_graph.get_route_count() << "\n";

    string target_name = "pardisan";
    int target_id = metro_graph.find_station_id(target_name);
    cout << "id of " << target_name << ": " << target_id << "\n";

    if (metro_graph.has_station(target_id))
    {
        station &target_station = metro_graph.get_station(target_id);
        cout << "neighbors of " << target_station.get_name() << ":\n";

        for (auto &e : metro_graph.get_neighbors(target_id))
        {
            station &neighbor_station = metro_graph.get_station(e.get_destination());
            cout << "    -> " << neighbor_station.get_name()
                 << " | distance: " << e.get_distance()
                 << " | time: " << e.get_time() << "\n";
        }
    }

    return 0;
}