#ifndef EDGE_H
#define EDGE_H

class edge
{
private:
    int destination;
    double distance;
    double time;

public:
    edge();
    edge(int destination_id, double distance_km, double time_min);

    int get_destination() ;
    double get_distance() ;
    double get_time() ;
};

#endif
