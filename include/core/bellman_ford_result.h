#pragma once

#include <vector>
using namespace std;

struct BellmanFordResult
{

    bool negative_cycle_detected;

    bool reach_able;
    vector<int> path;
    double total_cost;

    BellmanFordResult() : negative_cycle_detected(false), reach_able(false), path(), total_cost(0.0)
    {
    }
};
