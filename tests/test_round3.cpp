#include <iostream>
#include <vector>
#include <cmath>
#include <stdexcept>

#include "platform_scheduler.h"
#include "dispatch_queue.h"
#include "network_analytics.h"
#include "passenger_simulator.h"
#include "train.h"

using namespace std;

int passed = 0;
int failed = 0;

void check(bool condition, const string &test_name)
{
    if (condition)
    {
        cout << "[PASS] " << test_name << '\n';
        passed++;
    }
    else
    {
        cout << "[FAIL] " << test_name << '\n';
        failed++;
    }
}


// ============================================================
// T3.1 - Interval Scheduling
// ============================================================

void test_t3_1()
{
    cout << "\n========== T3.1 Platform Scheduler ==========\n";

    vector<train> trains = {
        train(1, 1, 3),
        train(2, 2, 5),
        train(3, 4, 6),
        train(4, 6, 8)
    };

    platform_scheduler scheduler;

    vector<train> selected = scheduler.schedule(trains);

    cout << "Selected trains: ";

    for (const auto &t : selected)
    {
        cout << t.get_id() << " ";
    }

    cout << '\n';

    check(
        selected.size() == 3,
        "T3.1 selects maximum number of compatible trains"
    );

    check(
        selected.size() == 3 &&
        selected[0].get_id() == 1 &&
        selected[1].get_id() == 3 &&
        selected[2].get_id() == 4,
        "T3.1 selected train IDs are 1, 3, 4"
    );


    vector<train> empty_input;

    check(
        scheduler.schedule(empty_input).empty(),
        "T3.1 handles empty input"
    );
}


// ============================================================
// T3.2 - Dispatch Queue / Min-Heap
// ============================================================

void test_t3_2()
{
    cout << "\n========== T3.2 Dispatch Queue ==========\n";

    dispatch_queue queue;

    queue.enqueue_train(train(1, 10, 30));
    queue.enqueue_train(train(2, 5, 15));
    queue.enqueue_train(train(3, 8, 20));

    check(
        queue.size() == 3,
        "T3.2 enqueue stores all trains"
    );

    train top = queue.peek_next();

    cout << "Highest priority train: "
         << top.get_id()
         << '\n';

    check(
        top.get_id() == 2,
        "T3.2 peek returns train with earliest departure"
    );

    train first = queue.dispatch_next();
    train second = queue.dispatch_next();
    train third = queue.dispatch_next();

    cout << "Dispatch order: "
         << first.get_id() << " "
         << second.get_id() << " "
         << third.get_id() << '\n';

    check(
        first.get_id() == 2 &&
        second.get_id() == 3 &&
        third.get_id() == 1,
        "T3.2 dispatch order follows priority"
    );

    check(
        queue.empty(),
        "T3.2 queue becomes empty after all dispatches"
    );

    bool exception_thrown = false;

    try
    {
        queue.dispatch_next();
    }
    catch (const std::out_of_range &)
    {
        exception_thrown = true;
    }

    check(
        exception_thrown,
        "T3.2 throws exception when dispatching from empty queue"
    );
}


// ============================================================
// T3.3 - Network Analytics
// ============================================================

void test_t3_3()
{
    cout << "\n========== T3.3 Network Analytics ==========\n";

    network_analytics analytics;

    // Day 1: 4 trips
    analytics.record_trip(1);
    analytics.record_trip(1);
    analytics.record_trip(2);
    analytics.record_trip(3);
    analytics.finish_day();

    // Day 2: 6 trips
    analytics.record_trip(1);
    analytics.record_trip(1);
    analytics.record_trip(1);
    analytics.record_trip(2);
    analytics.record_trip(2);
    analytics.record_trip(3);
    analytics.finish_day();

    // Total = 10 trips / 2 days = 5
    double average = analytics.average_daily_trips();

    cout << "Average daily trips: "
         << average
         << '\n';

    check(
        fabs(average - 5.0) < 0.0001,
        "T3.3 calculates average daily trips correctly"
    );

    int busiest = analytics.kth_busiest_station(1);
    int second_busiest = analytics.kth_busiest_station(2);

    cout << "Busiest station ID: "
         << busiest
         << '\n';

    cout << "Second busiest station ID: "
         << second_busiest
         << '\n';

    check(
        busiest == 1,
        "T3.3 finds busiest station correctly"
    );

    check(
        second_busiest == 2,
        "T3.3 finds second busiest station correctly"
    );

    bool exception_thrown = false;

    try
    {
        analytics.kth_busiest_station(10);
    }
    catch (const std::out_of_range &)
    {
        exception_thrown = true;
    }

    check(
        exception_thrown,
        "T3.3 rejects invalid k"
    );
}


// ============================================================
// T3.4 - Passenger Simulation
// ============================================================

void test_t3_4()
{
    cout << "\n========== T3.4 Passenger Simulation ==========\n";

    passenger_simulator simulator(2);

    // Three passengers arrive at time 0.
    simulator.generate_passengers(3, 0);

    check(
        simulator.queue_size() == 3,
        "T3.4 generates passengers correctly"
    );

    // Gate capacity = 2, so only two passengers are processed.
    simulator.process_gate(2);

    cout << "Queue after first processing: "
         << simulator.queue_size()
         << '\n';

    cout << "Processed passengers: "
         << simulator.processed_count()
         << '\n';

    check(
        simulator.processed_count() == 2,
        "T3.4 respects gate capacity"
    );

    check(
        simulator.queue_size() == 1,
        "T3.4 keeps unprocessed passengers in queue"
    );

    // Both processed passengers waited 2 units.
    check(
        fabs(simulator.average_waiting_time() - 2.0) < 0.0001,
        "T3.4 calculates initial average waiting time correctly"
    );

    // Remaining passenger is processed at time 5.
    simulator.process_gate(5);

    // Waiting times = 2, 2, 5 => average = 3
    cout << "Final average waiting time: "
         << simulator.average_waiting_time()
         << '\n';

    check(
        simulator.processed_count() == 3,
        "T3.4 processes remaining passenger"
    );

    check(
        simulator.queue_size() == 0,
        "T3.4 queue becomes empty"
    );

    check(
        fabs(simulator.average_waiting_time() - 3.0) < 0.0001,
        "T3.4 calculates final average waiting time correctly"
    );


    // Random passenger generation required by T3.4.
    passenger_simulator random_simulator(3);

    random_simulator.generate_random_passengers(5, 0);

    int random_count = random_simulator.queue_size();

    cout << "Random passengers generated: "
         << random_count
         << '\n';

    check(
        random_count >= 0 && random_count <= 5,
        "T3.4 random generation stays within requested range"
    );
}


// ============================================================
// Main Test Runner
// ============================================================

int main()
{
    cout << "============================================\n";
    cout << "       ROUND 3 TEST SUITE\n";
    cout << "============================================\n";

    test_t3_1();
    test_t3_2();
    test_t3_3();
    test_t3_4();

    cout << "\n============================================\n";
    cout << "TEST SUMMARY\n";
    cout << "============================================\n";

    cout << "Passed: " << passed << '\n';
    cout << "Failed: " << failed << '\n';

    if (failed == 0)
    {
        cout << "\nALL ROUND 3 TESTS PASSED\n";
        return 0;
    }

    cout << "\nSOME TESTS FAILED\n";
    return 1;
}