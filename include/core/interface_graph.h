#ifndef INTERFACE_GRAPH_H
#define INTERFACE_GRAPH_H

#include<bits/stdc++.h>
#include "station.h"
#include "edge.h"
using namespace std;
class interface_graph
{
public:
    virtual ~interface_graph()=default;


    virtual int add_station( string& station_name) = 0;

    
    virtual void add_route(int from_id, int to_id, double distance_km, double time_min) = 0;

   
    virtual int get_station_count()  = 0;

    virtual  vector<edge>& get_neighbors(int station_id)  = 0;

    virtual bool has_station(int station_id)  = 0;

  
    virtual int find_station_id( string& station_name)  = 0;

   
    virtual  station& get_station(int station_id)  = 0;

    
    virtual  vector<station>& get_all_stations()  = 0;
};

#endif 
