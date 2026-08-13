#pragma once

class directed_edge
{
private:
    int destination;
    double weight;

public:
    directed_edge();
    directed_edge(int destination_id, double weight_value);

    int get_destination();
    double get_weight();
};
