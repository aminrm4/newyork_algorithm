#include "station.h"

station::station()
    : id(-1), name(""), latitude(0.0), longitude(0.0)
{
}

station::station(int station_id, const string &station_name) : id(station_id), name(station_name), latitude(0.0), longitude(0.0)
{
}

station::station(int station_id, const string &station_name, double station_latitude, double station_longitude) : id(station_id), name(station_name), latitude(station_latitude), longitude(station_longitude)
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

double station::get_latitude()
{
    return latitude;
}

double station::get_longitude()
{
    return longitude;
}