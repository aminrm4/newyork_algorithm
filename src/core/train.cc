#include "train.h"
train::train()
    : id(-1), arrival_time(0), departure_time(0)
{}

train::train(int train_id, int arrival, int departure)
    : id(train_id), arrival_time(arrival), departure_time(departure)
{}

int train::get_id()const{return id;}
    

int train::get_arrival_time()const{return arrival_time;}
    

int train::get_departure_time()const{return departure_time;}
    

