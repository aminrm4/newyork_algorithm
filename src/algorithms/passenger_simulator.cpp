#include "passenger_simulator.h"

#include <algorithm>
#include <stdexcept>

passenger_simulator::passenger_simulator(int gate_capacity)
    : next_passenger_id(1),
      gate_capacity(gate_capacity),
      processed_passengers(0),
      total_waiting_time(0)
{
    if (gate_capacity <= 0)
    {
        throw std::invalid_argument("gate capacity must be positive");
    }
}

void passenger_simulator::generate_passengers(int count, int current_time)
{
    if (count < 0)
    {
        throw std::invalid_argument("passenger count cannot be negative");
    }

    if (current_time < 0)
    {
        throw std::invalid_argument("current time cannot be negative");
    }

    for (int i = 0; i < count; ++i)
    {
        gate_queue.push(
            passenger(next_passenger_id, current_time)
        );

        next_passenger_id++;
    }
}

void passenger_simulator::process_gate(int current_time)
{
    if (current_time < 0)
    {
        throw std::invalid_argument("current time cannot be negative");
    }

    int passengers_to_process =
        std::min(
            gate_capacity,
            static_cast<int>(gate_queue.size())
        );

    for (int i = 0; i < passengers_to_process; ++i)
    {
        passenger current_passenger = gate_queue.front();
        gate_queue.pop();

        current_passenger.set_service_start_time(current_time);

        total_waiting_time +=
            current_time - current_passenger.get_arrival_time();

        processed_passengers++;
    }
}

double passenger_simulator::average_waiting_time() const
{
    if (processed_passengers == 0)
    {
        return 0.0;
    }

    return static_cast<double>(total_waiting_time) /
           processed_passengers;
}

int passenger_simulator::queue_size() const
{
    return static_cast<int>(gate_queue.size());
}

int passenger_simulator::processed_count() const
{
    return processed_passengers;
}
void passenger_simulator::generate_random_passengers(
    int max_count,
    int current_time
)
{
    if (max_count < 0)
    {
        throw std::invalid_argument(
            "max passenger count cannot be negative"
        );
    }

    if (current_time < 0)
    {
        throw std::invalid_argument(
            "current time cannot be negative"
        );
    }

    static std::random_device rd;
    static std::mt19937 generator(rd());

    std::uniform_int_distribution<int> distribution(0, max_count);

    int count = distribution(generator);

    generate_passengers(count, current_time);
}