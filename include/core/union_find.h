#pragma once

#include <vector>
using namespace std;

class union_find
{
public:
    explicit union_find(int element_count);

    int find(int x);
    bool union_sets(int x, int y);
    bool connected(int x, int y);
    int get_set_size(int x);
    int get_component_count();

private:
    vector<int> parent;
    vector<int> rank_;
    vector<int> size_;
    int component_count;

    void validate_element(int x);
};
