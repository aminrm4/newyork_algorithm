#ifndef graph_h
#define graph_h

#include <bits/stdc++.h>
#include "interface_graph.h"
#include "station.h"
#include "edge.h"
using namespace std;
class graph : public interface_graph
{
public:
    graph();

    int add_station(const string &station_name) override;
    void add_route(int from_id, int to_id, double distance_km, double time_min) override;

    int get_station_count() override;
    vector<edge> &get_neighbors(int station_id) override;

    bool has_station(int station_id) override;
    int find_station_id(const string &station_name) override;
    station &get_station(int station_id) override;
    vector<station> &get_all_stations() override;

    int get_route_count();
    // write only for test , can be deleted

    void print_graph();

private:
    int route_count;

    vector<station> stations;
    vector<vector<edge>> adjacency_list;
    unordered_map<string, int> name_to_id;

    void validate_station_id(int station_id);
};

#endif