#pragma once
#include "interface_graph.h"

class dominating_set_approximation{
private:
    interface_graph& graph;
public:
    dominating_set_approximation(interface_graph& g);

    std::vector<int> find_dominating_set();



};