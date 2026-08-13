#pragma once

#include <vector>
#include "mst_edge.h"
using namespace std;

struct MSTResult
{
    bool is_spanning_tree;
    int component_count;

    vector<MSTEdge> edges;
    double total_weight;

    MSTResult() : is_spanning_tree(false), component_count(0), edges(), total_weight(0.0)
    {
    }
};
