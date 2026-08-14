#include "union_find.h"
#include <stdexcept>

union_find::union_find(int element_count)
    : parent(element_count), rank_(element_count, 0), size_(element_count, 1), component_count(element_count)
{
    for (int i = 0; i < element_count; i++)
    {
        parent[i] = i;
    }
}

void union_find::validate_element(int x)
{
    if (x < 0 || x >= static_cast<int>(parent.size()))
    {
        throw out_of_range("invalid union_find element");
    }
}

int union_find::find(int x)
{
    validate_element(x);

    int root = x;
    while (parent[root] != root)
    {
        root = parent[root];
    }

    while (parent[x] != root)
    {
        int next = parent[x];
        parent[x] = root;
        x = next;
    }

    return root;
}

bool union_find::union_sets(int x, int y)
{
    int root_x = find(x);
    int root_y = find(y);

    if (root_x == root_y)
    {
        return false;
    }

    if (rank_[root_x] < rank_[root_y])
    {
        swap(root_x, root_y);
    }

    parent[root_y] = root_x;
    size_[root_x] += size_[root_y];

    if (rank_[root_x] == rank_[root_y])
    {
        rank_[root_x]++;
    }

    component_count--;

    return true;
}

bool union_find::connected(int x, int y)
{
    return find(x) == find(y);
}

int union_find::get_set_size(int x)
{
    return size_[find(x)];
}

int union_find::get_component_count()
{
    return component_count;
}
