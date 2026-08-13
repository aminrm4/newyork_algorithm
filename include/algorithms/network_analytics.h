#pragma once

#include <unordered_map>

class network_analytics
{
public:
    network_analytics();

    void record_trip(int station_id);
    void finish_day();

    double average_daily_trips();
    int kth_busiest_station(int k);

private:
    std::unordered_map<int, int> station_trips;
    int total_trips;
    int simulated_days;
};