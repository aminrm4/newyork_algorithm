#pragma once

struct MSTEdge
{
    int from;
    int to;
    double weight;

    MSTEdge() : from(-1), to(-1), weight(0.0)
    {
    }

    MSTEdge(int from_id, int to_id, double weight_value)
        : from(from_id), to(to_id), weight(weight_value)
    {
    }
};