#include "directed_edge.h"

directed_edge::directed_edge()
    : destination(-1), weight(0.0)
{
}

directed_edge::directed_edge(int destination_id, double weight_value)
    : destination(destination_id), weight(weight_value)
{
}

int directed_edge::get_destination()
{
    return destination;
}

double directed_edge::get_weight()
{
    return weight;
}
