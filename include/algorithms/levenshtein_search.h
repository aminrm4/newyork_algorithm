#pragma once
#include "interface_graph.h"
#include <string>

class levenshtein_search {
private:
    interface_graph &graph;
        int levenshtein_distance(const std::string& first, const std::string& second);

public:
    levenshtein_search(interface_graph &graph);
    station find_closest(const std::string& name);
};