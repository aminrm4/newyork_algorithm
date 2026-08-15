#pragma once
#include "metro_system.h"

#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

#include <string>
#include <vector>

class metro_tui
{
private:
    metro_system& system;
    ftxui::ScreenInteractive screen;

    int selected_tab;
    int menu_selected;
    std::vector<std::string> menu_entries;

    // Station names for UI selection.
    // The selected index is the station ID used by metro_system.
    std::vector<std::string> station_names;

    ftxui::Component create_station_menu(
        const std::string& label,
        int* selected_index
    );

    int get_station_id(int selected_index) const;

    // network info screen
    std::string network_info_text;
    ftxui::Component build_network_info_screen();
    void refresh_network_info();

    // accessibility screen (BFS reachability)
    int accessibility_start_index;
    int accessibility_target_index;
    std::string accessibility_result;
    ftxui::Component build_accessibility_screen();
    void run_accessibility();

    // shortest path screen (Dijkstra)
    int shortest_start_index;
    int shortest_target_index;
    int shortest_metric_index;
    std::string shortest_result;
    ftxui::Component build_shortest_path_screen();
    void run_shortest_path();

    // A* vs Dijkstra comparison screen
    int astar_start_index;
    int astar_target_index;
    int astar_metric_index;
    std::string astar_result;
    ftxui::Component build_a_star_screen();
    void run_a_star_comparison();

    // MST comparison screen (Kruskal vs Prim)
    int mst_metric_index;
    std::string mst_result;
    ftxui::Component build_mst_screen();
    void run_mst_comparison();

    // express path screen (DAG shortest path)
    int express_start_index;
    int express_target_index;
    std::string express_result;
    ftxui::Component build_express_path_screen();
    void run_express_path();

    // incentive-aware path screen (Bellman-Ford)
    int incentive_start_index;
    int incentive_target_index;
    std::string incentive_result;
    ftxui::Component build_incentive_path_screen();
    void run_incentive_path();

    // platform scheduling screen
    std::string platform_count_input;
    std::vector<std::string> platform_arrivals;
    std::vector<std::string> platform_departures;
    ftxui::Component platform_rows_container;
    std::string platform_result;
    ftxui::Component build_platform_scheduling_screen();
    void regenerate_platform_rows();
    void run_platform_scheduling();

    // train dispatch queue screen
    std::string dispatch_count_input;
    std::vector<std::string> dispatch_arrivals;
    std::vector<std::string> dispatch_departures;
    ftxui::Component dispatch_rows_container;
    std::string dispatch_result;
    ftxui::Component build_dispatch_queue_screen();
    void regenerate_dispatch_rows();
    void run_dispatch_queue();

    // network analytics screen
    std::string analytics_trip_count_input;
    std::vector<std::string> analytics_trip_ids;
    ftxui::Component analytics_rows_container;
    std::string analytics_day_count_input;
    std::string analytics_k_input;
    std::string analytics_result;
    ftxui::Component build_network_analytics_screen();
    void regenerate_analytics_rows();
    void run_network_analytics();

    // passenger simulation screen
    std::string passenger_capacity_input;
    std::string passenger_steps_input;
    std::string passenger_max_arrivals_input;
    std::string passenger_result;
    ftxui::Component build_passenger_simulation_screen();
    void run_passenger_simulation();

    // Floyd-Warshall screen
    int floyd_start_index;
    int floyd_target_index;
    int floyd_metric_index;
    std::string floyd_result;
    ftxui::Component build_floyd_warshall_screen();
    void run_floyd_warshall();

    // max flow screen
    int flow_source_index;
    int flow_target_index;
    std::string flow_route_count_input;
    std::vector<int> flow_from_indices;
    std::vector<int> flow_to_indices;
    std::vector<std::string> flow_capacities;
    ftxui::Component flow_rows_container;
    std::string flow_result;
    ftxui::Component build_max_flow_screen();
    void regenerate_flow_rows();
    void run_max_flow();

    // critical stations screen
    std::string critical_result;
    ftxui::Component build_critical_stations_screen();
    void run_critical_stations();

    // emergency team placement screen
    std::string emergency_result;
    ftxui::Component build_emergency_team_screen();
    void run_emergency_team();

    // station search screen
    std::string search_query;
    std::string search_result;
    ftxui::Component build_station_search_screen();
    void run_station_search();

    ftxui::Component build_main_menu_screen();
    ftxui::Component wrap_screen(
        const std::string& title,
        ftxui::Component body,
        std::string* result_text
    );
    ftxui::Component labeled_input(
        const std::string& label,
        std::string* content,
        const std::string& placeholder
    );

    void go_back();
    route_metric metric_from_index(int index);
    bool try_parse_int(const std::string& text, int& out);
    bool try_parse_double(const std::string& text, double& out);

    std::vector<std::string> metric_labels;

public:
    explicit metro_tui(metro_system& system_ref);
    void run();
};