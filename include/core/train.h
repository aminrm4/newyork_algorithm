#pragma once



class train
{
private:
    int id;
    int arrival_time;
    int departure_time;

public:
    train();
    train(int train_id, int arrival, int departure);

    int get_id()const;
    int get_arrival_time()const;
    int get_departure_time()const;
};

