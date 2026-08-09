#include "platform_scheduler.h"
#include <algorithm>
using namespace std;
std::vector<train> platform_scheduler::schedule(std::vector<train> trains)
{
    std::vector<train> most;

    std::sort(
        trains.begin(),
        trains.end(),
        [](const train &a, const train &b)
        {
            return a.get_departure_time() < b.get_departure_time();
        }
    );

    if (trains.empty())
        return most;

    most.push_back(trains.at(0));

    int last_departure = most.at(0).get_departure_time();

    for (int i = 1; i < trains.size(); i++)
    {
        if (trains.at(i).get_arrival_time() >= last_departure)
        {
            most.push_back(trains.at(i));
            last_departure = trains.at(i).get_departure_time();
        }
    }

    return most;
}