#include "passenger.h"

passenger::passenger(){}

passenger::passenger(int id, int arrival_time):id(id), arrival_time(arrival_time), service_start_time(-1)
{
}

int passenger::get_id() const
{
    return id;
}

int passenger::get_arrival_time() const
{
    return arrival_time;
}

int passenger::get_service_start_time() const
{
    return service_start_time;
}

void passenger::set_service_start_time(int time)
{
    service_start_time= time;
}
