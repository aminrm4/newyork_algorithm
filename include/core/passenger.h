#pragma once
class passenger{
private:
    int id;
    int arrival_time;
    int service_start_time;
public:
    passenger();
    passenger( int id ,int arrival_time);

    int get_id()const;
    int get_arrival_time()const;
    int get_service_start_time() const;

    void set_service_start_time(int time);
};