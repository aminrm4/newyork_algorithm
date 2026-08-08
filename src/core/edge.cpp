#include "edge.h"

edge::edge()
    : destination(-1), distance(0.0), time(0.0)
{
}

edge::edge(int destination_id, double distance_km, double time_min) : destination(destination_id), distance(distance_km), time(time_min)
{
}

int edge::get_destination()
{
    return destination;
}

double edge::get_distance()
{
    return distance;
}

double edge::get_time()
{
    return time;
}
