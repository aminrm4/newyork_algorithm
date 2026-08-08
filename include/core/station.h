#ifndef STATION_H
#define STATION_H

#include <bits/stdc++.h>
using namespace std;

class station
{
private:
    int id;
    string name;

public:
    station();
    station(int station_id, string &station_name);

    int get_id();
    string & get_name();
};

#endif
