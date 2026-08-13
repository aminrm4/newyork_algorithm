#include "dominating_set_approximation.h"

using namespace std;

dominating_set_approximation::dominating_set_approximation(interface_graph &g)
    : graph(g)
{
}

std::vector<int> dominating_set_approximation::find_dominating_set()
{
    int n = graph.get_station_count();

    vector<bool> covered(n, false);
    vector<int> selected;

    int covered_count = 0;

    while (covered_count < n)
    {
        int best_station = -1;
        int best_gain = -1;

        for (int id = 0; id < n; id++)
        {
            int gain = 0;

            if (!covered[id])
            {
                gain++;
            }

            auto &neighbors = graph.get_neighbors(id);

            for (auto &e : neighbors)
            {
                int neighbor_id = e.get_destination();

                if (!covered[neighbor_id])
                {
                    gain++;
                }
            }

            if (gain > best_gain)
            {
                best_gain = gain;
                best_station = id;
            }
        }

        selected.push_back(best_station);

        if (!covered[best_station])
        {
            covered[best_station] = true;
            covered_count++;
        }

        auto &neighbors = graph.get_neighbors(best_station);

        for (auto &e : neighbors)
        {
            int neighbor_id = e.get_destination();

            if (!covered[neighbor_id])
            {
                covered[neighbor_id] = true;
                covered_count++;
            }
        }
    }

    return selected;
}