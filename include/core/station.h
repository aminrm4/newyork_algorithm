#ifndef station_h
#define station_h

#include <bits/stdc++.h>
using namespace std;

class station
{
private:
    int id;
    string name;
    double latitude;
    double longitude;

public:
    station();
    station(int station_id, const string &station_name);
    station(int station_id, const string &station_name, double station_latitude, double station_longitude);

    int get_id();
    string &get_name();
    double get_latitude();
    double get_longitude();
};

#endif