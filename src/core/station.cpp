#include "station.h"

station::station()
    : id(-1), name("")
{
}

station::station(int station_id, const string &station_name) : id(station_id), name(station_name)
{
}

int station::get_id()
{
    return id;
}

string &station::get_name()
{
    return name;
}