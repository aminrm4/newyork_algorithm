#pragma once

#include <queue>
#include "core/passenger.h"
#include <random>

class passenger_simulator
{
public:
    passenger_simulator(int gate_capacity);

    void generate_passengers(int count, int current_time);
    void process_gate(int current_time);

    double average_waiting_time() const;
    int queue_size() const;
    int processed_count() const;
    void generate_random_passengers(int max_count, int current_time);

private:
    std::queue<passenger> gate_queue;

    int next_passenger_id;
    int gate_capacity;

    int processed_passengers;
    long long total_waiting_time;
};