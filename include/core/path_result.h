#ifndef PATH_RESULT_H
#define PATH_RESULT_H

#include <bits/stdc++.h>
using namespace std;
struct PathResult
{
    bool reach_able;
    vector<int> path;
    double total_cost;
    int expanded_nodes;

    PathResult() : reach_able(false), path(), total_cost(0.0), expanded_nodes(0)
    {
    }
};

#endif