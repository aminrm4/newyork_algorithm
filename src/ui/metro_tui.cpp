#include "metro_tui.h"

#include <sstream>
#include <limits>
#include <stdexcept>

using namespace ftxui;

metro_tui::metro_tui(metro_system& system_ref)
    : system(system_ref),
      screen(ScreenInteractive::Fullscreen()),
      selected_tab(0),
      menu_selected(0),
      accessibility_start_index(0),
      accessibility_target_index(1),
      shortest_start_index(0),
      shortest_target_index(1),
      shortest_metric_index(0),
      astar_start_index(0),
      astar_target_index(1),
      astar_metric_index(0),
      mst_metric_index(0),
      express_start_index(0),
      express_target_index(1),
      incentive_start_index(0),
      incentive_target_index(1),
      floyd_start_index(0),
      floyd_target_index(1),
      floyd_metric_index(0),
      flow_source_index(0),
      flow_target_index(1)
{
    metric_labels = {"Distance", "Time"};

    menu_entries = {
        "Network Info",
        "Accessibility (BFS)",
        "Shortest Path (Dijkstra)",
        "A* vs Dijkstra",
        "Minimum Cost Network (MST)",
        "Express Path (DAG)",
        "Incentive-Aware Path (Bellman-Ford)",
        "Platform Scheduling",
        "Train Dispatch Queue",
        "Network Analytics",
        "Passenger Simulation",
        "All-Pairs Shortest Path (Floyd-Warshall)",
        "Max Flow",
        "Critical Stations",
        "Emergency Team Placement",
        "Station Search",
    };

    station_names.clear();

    for (int i = 0; i < system.get_station_count(); ++i)
        station_names.push_back(system.get_station_name(i));

    platform_rows_container = Container::Vertical({});
    dispatch_rows_container = Container::Vertical({});
    analytics_rows_container = Container::Vertical({});
    flow_rows_container = Container::Vertical({});

    refresh_network_info();
}

bool metro_tui::try_parse_int(const std::string& text, int& out)
{
    try
    {
        size_t pos;
        out = std::stoi(text, &pos);
        return pos == text.size();
    }
    catch (const std::exception&)
    {
        return false;
    }
}

bool metro_tui::try_parse_double(const std::string& text, double& out)
{
    try
    {
        size_t pos;
        out = std::stod(text, &pos);
        return pos == text.size();
    }
    catch (const std::exception&)
    {
        return false;
    }
}

route_metric metro_tui::metric_from_index(int index)
{
    return index == 0 ? route_metric::DISTANCE : route_metric::TIME;
}

void metro_tui::go_back()
{
    selected_tab = 0;
}

Component metro_tui::labeled_input(
    const std::string& label,
    std::string* content,
    const std::string& placeholder)
{
    auto input = Input(content, placeholder);

    return Renderer(input, [label, input]
    {
        return hbox({
            text(label) | size(WIDTH, EQUAL, 22),
            input->Render() | flex,
        });
    });
}

Component metro_tui::wrap_screen(
    const std::string& title,
    Component body,
    std::string* result_text)
{
    auto back_button =
        Button("Back to menu", [this] { go_back(); });

    auto layout =
        Container::Vertical({body, back_button});

    return Renderer(
        layout,
        [title, body, back_button, result_text]
        {
            return vbox({
                text(title) | bold | center,
                separator(),
                body->Render(),
                separator(),
                text("Result:") | bold,
                paragraph(*result_text) | border,
                separator(),
                back_button->Render(),
            }) | border;
        });
}

Component metro_tui::build_main_menu_screen()
{
    auto menu_option = MenuOption::Vertical();

    menu_option.on_enter = [this]
    {
        selected_tab = menu_selected + 1;
    };

    auto menu =
        Menu(&menu_entries, &menu_selected, menu_option);

    return Renderer(menu, [this, menu]
    {
        return vbox({
            text("Qom / New York Metro Routing System")
                | bold
                | center,
            separator(),
            text("Select a feature and press Enter:"),
            separator(),
            menu->Render() | frame,
        }) | border;
    });
}

void metro_tui::refresh_network_info()
{
    std::ostringstream out;

    out << "Stations: "
        << system.get_station_count()
        << "\n";

    out << "Routes: "
        << system.get_route_count();

    network_info_text = out.str();
}

Component metro_tui::build_network_info_screen()
{
    auto refresh_button =
        Button(
            "Refresh",
            [this]
            {
                refresh_network_info();
            });

    return wrap_screen(
        "Network Info",
        refresh_button,
        &network_info_text);
}

int metro_tui::get_station_id(int selected_index) const
{
    if (selected_index < 0 ||
        selected_index >=
            static_cast<int>(station_names.size()))
    {
        return -1;
    }

    return selected_index;
}

Component metro_tui::create_station_menu(
    const std::string& label,
    int* selected_index)
{
    auto menu =
        Menu(
            &station_names,
            selected_index);

    return Renderer(
        menu,
        [label, menu]
        {
            return vbox({
                text(label) | bold,
                menu->Render()
                    | frame
                    | border
            });
        });
}

void metro_tui::run_accessibility()
{
    int start_id =
        get_station_id(accessibility_start_index);

    int target_id =
        get_station_id(accessibility_target_index);

    if (start_id == -1 || target_id == -1)
    {
        accessibility_result =
            "Invalid station selection.";
        return;
    }

    try
    {
        PathResult result =
            system.find_path(
                start_id,
                target_id);

        if (!result.reach_able)
        {
            accessibility_result =
                "No path exists.";
            return;
        }

        std::ostringstream out;
        out << "Path: ";

        for (size_t i = 0;
             i < result.path.size();
             ++i)
        {
            out << system.get_station_name(
                result.path[i]);

            if (i + 1 < result.path.size())
                out << " -> ";
        }

        accessibility_result =
            out.str();
    }
    catch (const std::exception& e)
    {
        accessibility_result =
            std::string("Error: ") + e.what();
    }
}

Component metro_tui::build_accessibility_screen()
{
    auto start_menu =
        create_station_menu(
            "Start station:",
            &accessibility_start_index);

    auto target_menu =
        create_station_menu(
            "Target station:",
            &accessibility_target_index);

    auto run_button =
        Button(
            "Check reachability",
            [this]
            {
                run_accessibility();
            });

    auto body =
        Container::Vertical({
            start_menu,
            target_menu,
            run_button
        });

    return wrap_screen(
        "Accessibility (BFS)",
        body,
        &accessibility_result);
}

void metro_tui::run_shortest_path()
{
    int start_id =
        get_station_id(shortest_start_index);

    int target_id =
        get_station_id(shortest_target_index);

    if (start_id == -1 || target_id == -1)
    {
        shortest_result =
            "Invalid station selection.";
        return;
    }

    try
    {
        route_metric metric =
            metric_from_index(
                shortest_metric_index);

        PathResult result =
            system.find_shortest_path(
                start_id,
                target_id,
                metric);

        if (!result.reach_able)
        {
            shortest_result =
                "No path exists.";
            return;
        }

        std::ostringstream out;
        out << "Path: ";

        for (size_t i = 0;
             i < result.path.size();
             ++i)
        {
            out << system.get_station_name(
                result.path[i]);

            if (i + 1 < result.path.size())
                out << " -> ";
        }

        out << "\nTotal cost: "
            << result.total_cost;

        shortest_result =
            out.str();
    }
    catch (const std::exception& e)
    {
        shortest_result =
            std::string("Error: ") + e.what();
    }
}

Component metro_tui::build_shortest_path_screen()
{
    auto start_menu =
        create_station_menu(
            "Start station:",
            &shortest_start_index);

    auto target_menu =
        create_station_menu(
            "Target station:",
            &shortest_target_index);

    auto metric_selector =
        Radiobox(
            &metric_labels,
            &shortest_metric_index);

    auto run_button =
        Button(
            "Find shortest path",
            [this]
            {
                run_shortest_path();
            });

    auto body =
        Container::Vertical({
            start_menu,
            target_menu,
            metric_selector,
            run_button
        });

    return wrap_screen(
        "Shortest Path (Dijkstra)",
        body,
        &shortest_result);
}

void metro_tui::run_a_star_comparison()
{
    int start_id =
        get_station_id(astar_start_index);

    int target_id =
        get_station_id(astar_target_index);

    if (start_id == -1 || target_id == -1)
    {
        astar_result =
            "Invalid station selection.";
        return;
    }

    try
    {
        route_metric metric =
            metric_from_index(
                astar_metric_index);

        PathResult dijkstra_result =
            system.find_shortest_path(
                start_id,
                target_id,
                metric);

        PathResult a_star_result =
            system.find_a_star_path(
                start_id,
                target_id,
                metric);

        if (!a_star_result.reach_able)
        {
            astar_result =
                "No path exists.";
            return;
        }

        std::ostringstream out;

        out << "A* path: ";

        for (size_t i = 0;
             i < a_star_result.path.size();
             ++i)
        {
            out << system.get_station_name(
                a_star_result.path[i]);

            if (i + 1 <
                a_star_result.path.size())
            {
                out << " -> ";
            }
        }

        out << "\nA* total cost: "
            << a_star_result.total_cost;

        out << "\nA* expanded nodes: "
            << a_star_result.expanded_nodes;

        out << "\nDijkstra expanded nodes: "
            << dijkstra_result.expanded_nodes;

        astar_result =
            out.str();
    }
    catch (const std::exception& e)
    {
        astar_result =
            std::string("Error: ") + e.what();
    }
}

Component metro_tui::build_a_star_screen()
{
    auto start_menu =
        create_station_menu(
            "Start station:",
            &astar_start_index);

    auto target_menu =
        create_station_menu(
            "Target station:",
            &astar_target_index);

    auto metric_selector =
        Radiobox(
            &metric_labels,
            &astar_metric_index);

    auto run_button =
        Button(
            "Compare A* vs Dijkstra",
            [this]
            {
                run_a_star_comparison();
            });

    auto body =
        Container::Vertical({
            start_menu,
            target_menu,
            metric_selector,
            run_button
        });

    return wrap_screen(
        "A* vs Dijkstra",
        body,
        &astar_result);
}

void metro_tui::run_mst_comparison()
{
    route_metric metric =
        metric_from_index(
            mst_metric_index);

    MSTComparisonResult comparison =
        system.compare_mst_algorithms(metric);

    std::ostringstream out;

    out << "[Kruskal] Total cost: "
        << comparison.kruskal_result.total_weight
        << " | Edges: "
        << comparison.kruskal_result.edges.size()
        << " | Time: "
        << comparison.kruskal_time_ms
        << " ms\n";

    out << "[Prim]    Total cost: "
        << comparison.prim_result.total_weight
        << " | Edges: "
        << comparison.prim_result.edges.size()
        << " | Time: "
        << comparison.prim_time_ms
        << " ms\n";

    if (!comparison.kruskal_result.is_spanning_tree ||
        !comparison.prim_result.is_spanning_tree)
    {
        out << "Note: The graph is not connected; "
               "a complete spanning tree could not be created.\n";
    }

    out << "\nSelected edges (Kruskal):\n";

    for (auto& e :
         comparison.kruskal_result.edges)
    {
        out << "  "
            << system.get_station_name(e.from)
            << " -- "
            << system.get_station_name(e.to)
            << " ("
            << e.weight
            << ")\n";
    }

    mst_result =
        out.str();
}

Component metro_tui::build_mst_screen()
{
    auto metric_selector =
        Radiobox(
            &metric_labels,
            &mst_metric_index);

    auto run_button =
        Button(
            "Compare MST algorithms",
            [this]
            {
                run_mst_comparison();
            });

    auto body =
        Container::Vertical({
            metric_selector,
            run_button
        });

    return wrap_screen(
        "Minimum Cost Network (MST)",
        body,
        &mst_result);
}

void metro_tui::run_express_path()
{
    int start_id =
        get_station_id(
            express_start_index);

    int target_id =
        get_station_id(
            express_target_index);

    if (start_id == -1 || target_id == -1)
    {
        express_result =
            "Invalid station selection.";
        return;
    }

    try
    {
        PathResult result =
            system.find_express_path(
                start_id,
                target_id);

        if (!result.reach_able)
        {
            express_result =
                "No path exists on the express line.";
            return;
        }

        std::ostringstream out;
        out << "Path: ";

        for (size_t i = 0;
             i < result.path.size();
             ++i)
        {
            out << system.get_station_name(
                result.path[i]);

            if (i + 1 < result.path.size())
                out << " -> ";
        }

        out << "\nTotal time: "
            << result.total_cost
            << " minutes";

        express_result =
            out.str();
    }
    catch (const std::logic_error& e)
    {
        express_result =
            std::string("Express line error: ") +
            e.what();
    }
    catch (const std::exception& e)
    {
        express_result =
            std::string("Error: ") + e.what();
    }
}

Component metro_tui::build_express_path_screen()
{
    auto start_menu =
        create_station_menu(
            "Start station:",
            &express_start_index);

    auto target_menu =
        create_station_menu(
            "Target station:",
            &express_target_index);

    auto run_button =
        Button(
            "Find express path",
            [this]
            {
                run_express_path();
            });

    auto body =
        Container::Vertical({
            start_menu,
            target_menu,
            run_button
        });

    return wrap_screen(
        "Express Path (DAG)",
        body,
        &express_result);
}

void metro_tui::run_incentive_path()
{
    int start_id =
        get_station_id(
            incentive_start_index);

    int target_id =
        get_station_id(
            incentive_target_index);

    if (start_id == -1 || target_id == -1)
    {
        incentive_result =
            "Invalid station selection.";
        return;
    }

    try
    {
        BellmanFordResult result =
            system.find_incentive_aware_path(
                start_id,
                target_id);

        if (result.negative_cycle_detected)
        {
            incentive_result =
                "A negative cycle was detected "
                "in the network.";
            return;
        }

        if (!result.reach_able)
        {
            incentive_result =
                "No path exists between these stations.";
            return;
        }

        std::ostringstream out;

        out << "Path: ";

        for (size_t i = 0;
             i < result.path.size();
             ++i)
        {
            out << system.get_station_name(
                result.path[i]);

            if (i + 1 < result.path.size())
                out << " -> ";
        }

        out << "\nFinal cost (including incentives): "
            << result.total_cost;

        incentive_result =
            out.str();
    }
    catch (const std::exception& e)
    {
        incentive_result =
            std::string("Error: ") + e.what();
    }
}

Component metro_tui::build_incentive_path_screen()
{
    auto start_menu =
        create_station_menu(
            "Start station:",
            &incentive_start_index);

    auto target_menu =
        create_station_menu(
            "Target station:",
            &incentive_target_index);

    auto run_button =
        Button(
            "Find incentive-aware path",
            [this]
            {
                run_incentive_path();
            });

    auto body =
        Container::Vertical({
            start_menu,
            target_menu,
            run_button
        });

    return wrap_screen(
        "Incentive-Aware Path (Bellman-Ford)",
        body,
        &incentive_result);
}

void metro_tui::regenerate_platform_rows()
{
    int count;

    if (!try_parse_int(
            platform_count_input,
            count) ||
        count < 0)
    {
        platform_result =
            "Invalid number of trains.";
        return;
    }

    platform_arrivals.assign(
        count,
        "");

    platform_departures.assign(
        count,
        "");

    platform_rows_container
        ->DetachAllChildren();

    for (int i = 0;
         i < count;
         ++i)
    {
        auto arrival_input =
            labeled_input(
                "Train " +
                std::to_string(i + 1) +
                " arrival:",
                &platform_arrivals[i],
                "time");

        auto departure_input =
            labeled_input(
                "Train " +
                std::to_string(i + 1) +
                " departure:",
                &platform_departures[i],
                "time");

        auto row =
            Container::Horizontal({
                arrival_input,
                departure_input
            });

        platform_rows_container
            ->Add(row);
    }

    platform_result =
        "Rows generated. Fill them in and press Run.";
}

void metro_tui::run_platform_scheduling()
{
    std::vector<train> trains;

    for (size_t i = 0;
         i < platform_arrivals.size();
         ++i)
    {
        int arrival;
        int departure;

        if (!try_parse_int(
                platform_arrivals[i],
                arrival) ||
            !try_parse_int(
                platform_departures[i],
                departure))
        {
            platform_result =
                "Invalid time in row " +
                std::to_string(i + 1) +
                ".";
            return;
        }

        if (arrival >= departure)
        {
            platform_result =
                "Invalid interval in row " +
                std::to_string(i + 1) +
                ": arrival must be less than departure.";
            return;
        }

        trains.emplace_back(
            static_cast<int>(i) + 1,
            arrival,
            departure);
    }

    std::vector<train> selected_trains =
        system.schedule_trains(trains);

    std::ostringstream out;

    out << "Selected trains:\n";

    for (const auto& t :
         selected_trains)
    {
        out << "Train "
            << t.get_id()
            << " ["
            << t.get_arrival_time()
            << ", "
            << t.get_departure_time()
            << "]\n";
    }

    out << "Maximum number of trains: "
        << selected_trains.size();

    platform_result =
        out.str();
}

Component metro_tui::build_platform_scheduling_screen()
{
    auto count_input =
        labeled_input(
            "Number of trains:",
            &platform_count_input,
            "count");

    auto generate_button =
        Button(
            "Generate rows",
            [this]
            {
                regenerate_platform_rows();
            });

    auto run_button =
        Button(
            "Run scheduling",
            [this]
            {
                run_platform_scheduling();
            });

    auto body =
        Container::Vertical({
            count_input,
            generate_button,
            platform_rows_container,
            run_button
        });

    return wrap_screen(
        "Platform Scheduling (Max Trains)",
        body,
        &platform_result);
}

void metro_tui::regenerate_dispatch_rows()
{
    int count;

    if (!try_parse_int(
            dispatch_count_input,
            count) ||
        count < 0)
    {
        dispatch_result =
            "Invalid number of trains.";
        return;
    }

    dispatch_arrivals.assign(
        count,
        "");

    dispatch_departures.assign(
        count,
        "");

    dispatch_rows_container
        ->DetachAllChildren();

    for (int i = 0;
         i < count;
         ++i)
    {
        auto arrival_input =
            labeled_input(
                "Train " +
                std::to_string(i + 1) +
                " arrival:",
                &dispatch_arrivals[i],
                "time");

        auto departure_input =
            labeled_input(
                "Train " +
                std::to_string(i + 1) +
                " departure:",
                &dispatch_departures[i],
                "time");

        auto row =
            Container::Horizontal({
                arrival_input,
                departure_input
            });

        dispatch_rows_container
            ->Add(row);
    }

    dispatch_result =
        "Rows generated. Fill them in and press Run.";
}

void metro_tui::run_dispatch_queue()
{
    for (size_t i = 0;
         i < dispatch_arrivals.size();
         ++i)
    {
        int arrival;
        int departure;

        if (!try_parse_int(
                dispatch_arrivals[i],
                arrival) ||
            !try_parse_int(
                dispatch_departures[i],
                departure))
        {
            dispatch_result =
                "Invalid time in row " +
                std::to_string(i + 1) +
                ".";
            return;
        }

        if (arrival >= departure)
        {
            dispatch_result =
                "Invalid interval in row " +
                std::to_string(i + 1) +
                ".";
            return;
        }

        train t(
            static_cast<int>(i) + 1,
            arrival,
            departure);

        system.enqueue_train(t);
    }

    std::ostringstream out;

    out << "Number of trains in queue: "
        << system.dispatch_queue_size()
        << "\n";

    if (system.dispatch_queue_empty())
    {
        out << "Dispatch queue is empty.";
        dispatch_result =
            out.str();
        return;
    }

    train next =
        system.peek_next_train();

    out << "Highest priority train: Train "
        << next.get_id()
        << " ["
        << next.get_arrival_time()
        << ", "
        << next.get_departure_time()
        << "]\n";

    out << "Dispatch order:\n";

    while (!system.dispatch_queue_empty())
    {
        train dispatched =
            system.dispatch_next_train();

        out << "Train "
            << dispatched.get_id()
            << " ["
            << dispatched.get_arrival_time()
            << ", "
            << dispatched.get_departure_time()
            << "]\n";
    }

    dispatch_result =
        out.str();
}

Component metro_tui::build_dispatch_queue_screen()
{
    auto count_input =
        labeled_input(
            "Number of trains:",
            &dispatch_count_input,
            "count");

    auto generate_button =
        Button(
            "Generate rows",
            [this]
            {
                regenerate_dispatch_rows();
            });

    auto run_button =
        Button(
            "Enqueue and dispatch",
            [this]
            {
                run_dispatch_queue();
            });

    auto body =
        Container::Vertical({
            count_input,
            generate_button,
            dispatch_rows_container,
            run_button
        });

    return wrap_screen(
        "Train Dispatch Priority Queue",
        body,
        &dispatch_result);
}

void metro_tui::regenerate_analytics_rows()
{
    int count;

    if (!try_parse_int(
            analytics_trip_count_input,
            count) ||
        count < 0)
    {
        analytics_result =
            "Invalid number of trips.";
        return;
    }

    analytics_trip_ids.assign(
        count,
        "");

    analytics_rows_container
        ->DetachAllChildren();

    for (int i = 0;
         i < count;
         ++i)
    {
        auto trip_input =
            labeled_input(
                "Trip " +
                std::to_string(i + 1) +
                " station:",
                &analytics_trip_ids[i],
                "station ID");

        analytics_rows_container
            ->Add(trip_input);
    }

    analytics_result =
        "Rows generated. Fill them in and press Run.";
}

void metro_tui::run_network_analytics()
{
    for (size_t i = 0;
         i < analytics_trip_ids.size();
         ++i)
    {
        int station_id;

        if (!try_parse_int(
                analytics_trip_ids[i],
                station_id))
        {
            analytics_result =
                "Invalid station ID in trip " +
                std::to_string(i + 1) +
                ".";
            return;
        }

        try
        {
            system.record_trip(station_id);
        }
        catch (const std::out_of_range&)
        {
            analytics_result =
                "Invalid station ID in trip " +
                std::to_string(i + 1) +
                ".";
            return;
        }
    }

    int day_count;

    if (!try_parse_int(
            analytics_day_count_input,
            day_count) ||
        day_count < 0)
    {
        analytics_result =
            "Invalid number of days.";
        return;
    }

    for (int i = 0;
         i < day_count;
         ++i)
    {
        system.finish_day();
    }

    std::ostringstream out;

    out << "Average daily trips: "
        << system.get_avg_daily_trips()
        << "\n";

    int k;

    if (!try_parse_int(
            analytics_k_input,
            k))
    {
        out << "Invalid k.";
        analytics_result =
            out.str();
        return;
    }

    try
    {
        int station_id =
            system.get_kth_busiest_station(k);

        out << k
            << "th busiest station: "
            << system.get_station_name(station_id)
            << " (ID: "
            << station_id
            << ")";
    }
    catch (const std::out_of_range&)
    {
        out << "Invalid k.";
    }

    analytics_result =
        out.str();
}

Component metro_tui::build_network_analytics_screen()
{
    auto trip_count_input =
        labeled_input(
            "Number of trips:",
            &analytics_trip_count_input,
            "count");

    auto generate_button =
        Button(
            "Generate trip rows",
            [this]
            {
                regenerate_analytics_rows();
            });

    auto day_count_input =
        labeled_input(
            "Number of days completed:",
            &analytics_day_count_input,
            "days");

    auto k_input =
        labeled_input(
            "k for kth busiest station:",
            &analytics_k_input,
            "k");

    auto run_button =
        Button(
            "Run analytics",
            [this]
            {
                run_network_analytics();
            });

    auto body =
        Container::Vertical({
            trip_count_input,
            generate_button,
            analytics_rows_container,
            day_count_input,
            k_input,
            run_button,
        });

    return wrap_screen(
        "Network Operations Analytics",
        body,
        &analytics_result);
}

void metro_tui::run_passenger_simulation()
{
    int gate_capacity;
    int time_steps;
    int max_arrivals_per_step;

    if (!try_parse_int(
            passenger_capacity_input,
            gate_capacity) ||
        gate_capacity <= 0)
    {
        passenger_result =
            "Invalid gate capacity.";
        return;
    }

    if (!try_parse_int(
            passenger_steps_input,
            time_steps) ||
        time_steps <= 0)
    {
        passenger_result =
            "Invalid number of time steps.";
        return;
    }

    if (!try_parse_int(
            passenger_max_arrivals_input,
            max_arrivals_per_step) ||
        max_arrivals_per_step < 0)
    {
        passenger_result =
            "Invalid maximum arrivals.";
        return;
    }

    try
    {
        system.configure_gate(
            gate_capacity);
    }
    catch (const std::invalid_argument& e)
    {
        passenger_result =
            std::string(
                "Configuration error: ") +
            e.what();
        return;
    }

    std::ostringstream out;

    out << "Time | Queue size (after processing) | "
           "Total processed so far\n";

    out << "------------------------------------------------------------\n";

    for (int t = 0;
         t < time_steps;
         ++t)
    {
        try
        {
            system.simulate_passenger_arrivals(
                t,
                max_arrivals_per_step);

            system.process_passenger_gate(t);
        }
        catch (const std::invalid_argument& e)
        {
            out << "Simulation error at t = "
                << t
                << ": "
                << e.what();

            passenger_result =
                out.str();

            return;
        }

        out << "t = "
            << t
            << "   | "
            << system.get_gate_queue_size()
            << "\t| "
            << system.get_processed_passenger_count()
            << "\n";
    }

    out << "\n--- Simulation Summary ---\n";

    out << "Total passengers processed: "
        << system.get_processed_passenger_count()
        << "\n";

    out << "Passengers still waiting in queue: "
        << system.get_gate_queue_size()
        << "\n";

    out << "Average waiting time: "
        << system.get_avg_passenger_waiting_time()
        << " time units";

    passenger_result =
        out.str();
}

Component metro_tui::build_passenger_simulation_screen()
{
    auto capacity_input =
        labeled_input(
            "Gate capacity per step:",
            &passenger_capacity_input,
            "capacity");

    auto steps_input =
        labeled_input(
            "Number of time steps:",
            &passenger_steps_input,
            "steps");

    auto max_arrivals_input =
        labeled_input(
            "Max random arrivals per step:",
            &passenger_max_arrivals_input,
            "arrivals");

    auto run_button =
        Button(
            "Run simulation",
            [this]
            {
                run_passenger_simulation();
            });

    auto body =
        Container::Vertical({
            capacity_input,
            steps_input,
            max_arrivals_input,
            run_button
        });

    return wrap_screen(
        "Passenger Arrival Simulation",
        body,
        &passenger_result);
}

void metro_tui::run_floyd_warshall()
{
    int start_id =
        get_station_id(
            floyd_start_index);

    int target_id =
        get_station_id(
            floyd_target_index);

    if (start_id == -1 || target_id == -1)
    {
        floyd_result =
            "Invalid station selection.";
        return;
    }

    try
    {
        route_metric metric =
            metric_from_index(
                floyd_metric_index);

        double shortest_path =
            system.get_all_pairs_shortest_path(
                start_id,
                target_id,
                metric);

        if (shortest_path ==
            std::numeric_limits<double>::infinity())
        {
            floyd_result =
                "No path exists between these stations.";
            return;
        }

        std::ostringstream out;

        out << "Shortest path from "
            << system.get_station_name(start_id)
            << " to "
            << system.get_station_name(target_id)
            << ": "
            << shortest_path;

        out <<
            (metric == route_metric::DISTANCE
                ? " distance units"
                : " minutes");

        floyd_result =
            out.str();
    }
    catch (const std::exception& e)
    {
        floyd_result =
            std::string("Error: ") + e.what();
    }
}

Component metro_tui::build_floyd_warshall_screen()
{
    auto start_menu =
        create_station_menu(
            "Start station:",
            &floyd_start_index);

    auto target_menu =
        create_station_menu(
            "Target station:",
            &floyd_target_index);

    auto metric_selector =
        Radiobox(
            &metric_labels,
            &floyd_metric_index);

    auto run_button =
        Button(
            "Find all-pairs shortest path",
            [this]
            {
                run_floyd_warshall();
            });

    auto body =
        Container::Vertical({
            start_menu,
            target_menu,
            metric_selector,
            run_button
        });

    return wrap_screen(
        "All-Pairs Shortest Path (Floyd-Warshall)",
        body,
        &floyd_result);
}

void metro_tui::regenerate_flow_rows()
{
    int count;

    if (!try_parse_int(
            flow_route_count_input,
            count) ||
        count < 0)
    {
        flow_result =
            "Invalid route count.";
        return;
    }

    flow_from_indices.assign(
        count,
        0);

    flow_to_indices.assign(
        count,
        0);

    flow_capacities.assign(
        count,
        "");

    flow_rows_container
        ->DetachAllChildren();

    for (int i = 0;
         i < count;
         ++i)
    {
        auto from_menu =
            create_station_menu(
                "Route " +
                std::to_string(i + 1) +
                " from:",
                &flow_from_indices[i]);

        auto to_menu =
            create_station_menu(
                "Route " +
                std::to_string(i + 1) +
                " to:",
                &flow_to_indices[i]);

        auto capacity_input =
            labeled_input(
                "Route " +
                std::to_string(i + 1) +
                " capacity:",
                &flow_capacities[i],
                "capacity");

        auto row =
            Container::Vertical({
                from_menu,
                to_menu,
                capacity_input
            });

        flow_rows_container
            ->Add(row);
    }

    flow_result =
        "Rows generated. Select stations, enter capacities and press Run.";
}

void metro_tui::run_max_flow()
{
    int source_id =
        get_station_id(
            flow_source_index);

    int target_id =
        get_station_id(
            flow_target_index);

    if (source_id == -1 || target_id == -1)
    {
        flow_result =
            "Invalid station selection.";
        return;
    }

    try
    {
        for (size_t i = 0;
             i < flow_from_indices.size();
             ++i)
        {
            int from_id =
                get_station_id(
                    flow_from_indices[i]);

            int to_id =
                get_station_id(
                    flow_to_indices[i]);

            double capacity;

            if (from_id == -1 ||
                to_id == -1 ||
                !try_parse_double(
                    flow_capacities[i],
                    capacity))
            {
                flow_result =
                    "Invalid input in route " +
                    std::to_string(i + 1) +
                    ".";
                return;
            }

            if (capacity < 0)
            {
                flow_result =
                    "Capacity cannot be negative (route " +
                    std::to_string(i + 1) +
                    ").";
                return;
            }

            system.set_route_capacity(
                from_id,
                to_id,
                capacity);
        }

        max_flow_result result =
            system.find_max_passengers(
                source_id,
                target_id);

        flow_result =
            "Maximum passengers transferable: " +
            std::to_string(
                result.total_flow);
    }
    catch (const std::out_of_range&)
    {
        flow_result =
            "Invalid station selection.";
    }
    catch (const std::invalid_argument& e)
    {
        flow_result =
            std::string("Invalid input: ") +
            e.what();
    }
}

Component metro_tui::build_max_flow_screen()
{
    auto source_menu =
        create_station_menu(
            "Source station:",
            &flow_source_index);

    auto target_menu =
        create_station_menu(
            "Target station:",
            &flow_target_index);

    auto count_input =
        labeled_input(
            "Number of custom routes:",
            &flow_route_count_input,
            "count");

    auto generate_button =
        Button(
            "Generate route rows",
            [this]
            {
                regenerate_flow_rows();
            });

    auto run_button =
        Button(
            "Compute max flow",
            [this]
            {
                run_max_flow();
            });

    auto body =
        Container::Vertical({
            source_menu,
            target_menu,
            count_input,
            generate_button,
            flow_rows_container,
            run_button,
        });

    return wrap_screen(
        "Network Capacity During Peak Hours (Max Flow)",
        body,
        &flow_result);
}

void metro_tui::run_critical_stations()
{
    articulation_result result =
        system.find_critical_stations();

    std::ostringstream out;

    out << "Articulation Points:\n";

    if (result.articulation_points.empty())
    {
        out << "None\n";
    }
    else
    {
        for (int station_id :
             result.articulation_points)
        {
            out << "  "
                << system.get_station_name(station_id)
                << " (ID: "
                << station_id
                << ")\n";
        }
    }

    out << "\nBridges:\n";

    if (result.bridges.empty())
    {
        out << "None";
    }
    else
    {
        for (const auto& bridge :
             result.bridges)
        {
            out << "  "
                << system.get_station_name(bridge.first)
                << " -- "
                << system.get_station_name(bridge.second)
                << "\n";
        }
    }

    critical_result =
        out.str();
}

Component metro_tui::build_critical_stations_screen()
{
    auto run_button =
        Button(
            "Find critical stations",
            [this]
            {
                run_critical_stations();
            });

    return wrap_screen(
        "Critical Stations and Bridges",
        run_button,
        &critical_result);
}

void metro_tui::run_emergency_team()
{
    std::vector<int> selected =
        system.find_emergency_team_stations();

    std::ostringstream out;

    out << "Selected stations for emergency teams:\n";

    for (int station_id :
         selected)
    {
        out << "  "
            << system.get_station_name(station_id)
            << " (ID: "
            << station_id
            << ")\n";
    }

    out << "Number of emergency teams: "
        << selected.size();

    emergency_result =
        out.str();
}

Component metro_tui::build_emergency_team_screen()
{
    auto run_button =
        Button(
            "Find emergency team stations",
            [this]
            {
                run_emergency_team();
            });

    return wrap_screen(
        "Emergency Team Placement",
        run_button,
        &emergency_result);
}

void metro_tui::run_station_search()
{
    try
    {
        station result =
            system.find_closest_station(
                search_query);

        std::ostringstream out;

        out << "Closest station:\n";
        out << "Station ID: "
            << result.get_id()
            << "\n";

        out << "Station name: "
            << result.get_name();

        search_result =
            out.str();
    }
    catch (const std::exception& e)
    {
        search_result =
            std::string("Search failed: ") +
            e.what();
    }
}

Component metro_tui::build_station_search_screen()
{
    auto query_input =
        labeled_input(
            "Station name:",
            &search_query,
            "name");

    auto run_button =
        Button(
            "Search",
            [this]
            {
                run_station_search();
            });

    auto body =
        Container::Vertical({
            query_input,
            run_button
        });

    return wrap_screen(
        "Station Search with Typo Tolerance",
        body,
        &search_result);
}

void metro_tui::run()
{
    auto main_menu =
        build_main_menu_screen();

    Components screens = {
        main_menu,
        build_network_info_screen(),
        build_accessibility_screen(),
        build_shortest_path_screen(),
        build_a_star_screen(),
        build_mst_screen(),
        build_express_path_screen(),
        build_incentive_path_screen(),
        build_platform_scheduling_screen(),
        build_dispatch_queue_screen(),
        build_network_analytics_screen(),
        build_passenger_simulation_screen(),
        build_floyd_warshall_screen(),
        build_max_flow_screen(),
        build_critical_stations_screen(),
        build_emergency_team_screen(),
        build_station_search_screen(),
    };

    auto root =
        Container::Tab(
            screens,
            &selected_tab);

    root =
        CatchEvent(
            root,
            [this](Event event)
            {
                if (event == Event::Escape)
                {
                    if (selected_tab != 0)
                    {
                        go_back();
                        return true;
                    }

                    screen.Exit();
                    return true;
                }

                return false;
            });

    screen.Loop(root);
}