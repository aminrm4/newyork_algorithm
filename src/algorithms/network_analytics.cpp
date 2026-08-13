#include "network_analytics.h"

#include <algorithm>
#include <stdexcept>
#include <vector>
#include <utility>

network_analytics::network_analytics()
    : total_trips(0),
      simulated_days(0)
{
}

void network_analytics::record_trip(int station_id)
{
    station_trips[station_id]++;
    total_trips++;
}

void network_analytics::finish_day()
{
    simulated_days++;
}

double network_analytics::average_daily_trips()
{
    if (simulated_days == 0)
    {
        return 0.0;
    }

    return static_cast<double>(total_trips) / simulated_days;
}

int network_analytics::kth_busiest_station(int k)
{
    if (k <= 0 || k > static_cast<int>(station_trips.size()))
    {
        throw std::out_of_range("invalid k");
    }

    std::vector<std::pair<int, int>> stations(
        station_trips.begin(),
        station_trips.end()
    );

    std::sort(
        stations.begin(),
        stations.end(),
        [](const auto& a, const auto& b)
        {
            if (a.second != b.second)
                return a.second > b.second;

            return a.first < b.first;
        }
    );

    return stations[k - 1].first;
}