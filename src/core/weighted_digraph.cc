#include "weighted_digraph.h"
#include <stdexcept>

weighted_digraph::weighted_digraph()
    : vertex_count(0), adjacency_list()
{
}

weighted_digraph::weighted_digraph(int vertex_count)
    : vertex_count(vertex_count), adjacency_list(vertex_count)
{
}

void weighted_digraph::resize(int new_vertex_count)
{
    vertex_count = new_vertex_count;
    adjacency_list.assign(new_vertex_count, std::vector<directed_edge>());
}

void weighted_digraph::validate_vertex_id(int vertex_id)
{
    if (!has_vertex(vertex_id))
    {
        throw std::out_of_range("invalid vertex_id");
    }
}

void weighted_digraph::add_edge(int from_id, int to_id, double weight)
{
    validate_vertex_id(from_id);
    validate_vertex_id(to_id);

    adjacency_list[from_id].emplace_back(to_id, weight);
}

int weighted_digraph::get_vertex_count()
{
    return vertex_count;
}

bool weighted_digraph::has_vertex(int vertex_id)
{
    return vertex_id >= 0 && vertex_id < vertex_count;
}

std::vector<directed_edge> &weighted_digraph::get_neighbors(int vertex_id)
{
    validate_vertex_id(vertex_id);
    return adjacency_list[vertex_id];
}

std::vector<WeightedEdgeRecord> weighted_digraph::get_all_edges()
{
    std::vector<WeightedEdgeRecord> edges;

    for (int from_id = 0; from_id < vertex_count; from_id++)
    {
        for (auto &e : adjacency_list[from_id])
        {
            edges.emplace_back(from_id, e.get_destination(), e.get_weight());
        }
    }

    return edges;
}
