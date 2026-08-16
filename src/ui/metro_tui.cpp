#include "metro_tui.h"
#include <bits/stdc++.h>
#include <sstream>
#include <limits>
#include <stdexcept>
#include <ftxui/screen/box.hpp>
namespace
{
   class drag_scroll_base : public ftxui::ComponentBase
{
public:
    explicit drag_scroll_base(ftxui::Component child)
    {
        Add(std::move(child));
    }

    ftxui::Element Render() override
    {
        return ChildAt(0)->Render()
             | ftxui::focusPositionRelative(scroll_ratio_x_, scroll_ratio_y_)
             | ftxui::frame
             | ftxui::reflect(box_);
    }

    bool OnEvent(ftxui::Event event) override
    {
        if (!event.is_mouse())
            return ftxui::ComponentBase::OnEvent(event);

        return OnMouseEvent(event);
    }

private:
    bool OnMouseEvent(ftxui::Event event)
    {
        ftxui::Mouse mouse = event.mouse();

        bool handled_by_child = ftxui::ComponentBase::OnEvent(event);

        if (mouse.motion == ftxui::Mouse::Released)
        {
            dragging_ = false;
            return handled_by_child;
        }

        if (handled_by_child)
            return true;

        if (mouse.button != ftxui::Mouse::Left || mouse.motion != ftxui::Mouse::Pressed)
            return false;

        if (!box_.Contain(mouse.x, mouse.y))
            return false;

        if (!dragging_)
        {
            dragging_ = true;
            drag_start_x_ = mouse.x;
            drag_start_y_ = mouse.y;
            drag_start_ratio_x_ = scroll_ratio_x_;
            drag_start_ratio_y_ = scroll_ratio_y_;
            return true;
        }

        int width = std::max(1, box_.x_max - box_.x_min);
        int height = std::max(1, box_.y_max - box_.y_min);
        int delta_x = mouse.x - drag_start_x_;
        int delta_y = mouse.y - drag_start_y_;

        scroll_ratio_x_ = drag_start_ratio_x_ - static_cast<double>(delta_x) / static_cast<double>(width);
        scroll_ratio_y_ = drag_start_ratio_y_ - static_cast<double>(delta_y) / static_cast<double>(height);
        scroll_ratio_x_ = std::max(0.0, std::min(1.0, scroll_ratio_x_));
        scroll_ratio_y_ = std::max(0.0, std::min(1.0, scroll_ratio_y_));
        return true;
    }

    ftxui::Box box_;
    double scroll_ratio_x_ = 0.0;
    double scroll_ratio_y_ = 0.0;
    bool dragging_ = false;
    int drag_start_x_ = 0;
    int drag_start_y_ = 0;
    double drag_start_ratio_x_ = 0.0;
    double drag_start_ratio_y_ = 0.0;
};

ftxui::Component drag_scroll(ftxui::Component child)
{
    return ftxui::Make<drag_scroll_base>(std::move(child));
}

ftxui::Element render_multiline(const std::string &content);

class result_box_base : public ftxui::ComponentBase
{
public:
    result_box_base(const std::string *content, int max_height)
        : content_(content), max_height_(max_height)
    {
    }

    ftxui::Element Render() override
    {
        return render_multiline(*content_)
             | ftxui::focusPositionRelative(0.0, scroll_ratio_y_)
             | ftxui::vscroll_indicator
             | ftxui::frame
             | ftxui::reflect(box_)
             | ftxui::size(ftxui::HEIGHT, ftxui::LESS_THAN, max_height_)
             | ftxui::border;
    }

    bool OnEvent(ftxui::Event event) override
    {
        if (!event.is_mouse())
            return ftxui::ComponentBase::OnEvent(event);

        return OnMouseEvent(event);
    }

private:
    bool OnMouseEvent(ftxui::Event event)
    {
        ftxui::Mouse mouse = event.mouse();

        if (mouse.motion == ftxui::Mouse::Released)
        {
            dragging_ = false;
        }

        if (!box_.Contain(mouse.x, mouse.y))
            return false;

        if (mouse.button == ftxui::Mouse::WheelDown)
        {
            scroll_ratio_y_ = std::min(1.0, scroll_ratio_y_ + 0.08);
            return true;
        }

        if (mouse.button == ftxui::Mouse::WheelUp)
        {
            scroll_ratio_y_ = std::max(0.0, scroll_ratio_y_ - 0.08);
            return true;
        }

        if (mouse.button != ftxui::Mouse::Left)
            return false;

        if (mouse.motion == ftxui::Mouse::Pressed && !dragging_)
        {
            dragging_ = true;
            drag_start_y_ = mouse.y;
            drag_start_ratio_y_ = scroll_ratio_y_;
            return true;
        }

        if (mouse.motion == ftxui::Mouse::Pressed && dragging_)
        {
            int height = std::max(1, box_.y_max - box_.y_min);
            int delta_y = mouse.y - drag_start_y_;

            scroll_ratio_y_ =
                drag_start_ratio_y_ -
                static_cast<double>(delta_y) / static_cast<double>(height);
            scroll_ratio_y_ = std::max(0.0, std::min(1.0, scroll_ratio_y_));
            return true;
        }

        return false;
    }

    const std::string *content_;
    int max_height_;
    ftxui::Box box_;
    double scroll_ratio_y_ = 0.0;
    bool dragging_ = false;
    int drag_start_y_ = 0;
    double drag_start_ratio_y_ = 0.0;
};

ftxui::Component result_box(const std::string *content, int max_height)
{
    return ftxui::Make<result_box_base>(content, max_height);
}

    
    ftxui::Element render_multiline(const std::string &content)
    {
        std::vector<std::string> lines;
        std::string current;

        for (char c : content)
        {
            if (c == '\n')
            {
                lines.push_back(current);
                current.clear();
            }
            else
            {
                current.push_back(c);
            }
        }
        lines.push_back(current);

        ftxui::Elements rendered_lines;
        for (const auto &line : lines)
            rendered_lines.push_back(ftxui::text(line));

        return ftxui::vbox(std::move(rendered_lines));
    }

    ftxui::Component make_scrollable_rows(
        ftxui::Component child,
        int max_height)
    {
        return ftxui::Renderer(
            child,
            [child, max_height]
            {
                return child->Render()
                       | ftxui::vscroll_indicator
                       | ftxui::frame
                       | ftxui::size(
                             ftxui::HEIGHT,
                             ftxui::LESS_THAN,
                             max_height)
                       | ftxui::border;
            });
    }
}
using namespace ftxui;
const Color title_color = Color::Cyan;
const Color section_color = Color::Yellow;
const Color action_color = Color::Green;
const Color back_color = Color::Red;
const Color input_color = Color::White;
const Color result_color = Color::White;
const Color info_color = Color::GrayLight;

metro_tui::metro_tui(metro_system &system_ref)
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
      flow_target_index(1),
      flow_current_route_index(0),
      flow_editor_from_index(0),
      flow_editor_to_index(1)
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

bool metro_tui::try_parse_int(const std::string &text, int &out)
{
    try
    {
        size_t pos;
        out = std::stoi(text, &pos);
        return pos == text.size();
    }
    catch (const std::exception &)
    {
        return false;
    }
}

bool metro_tui::try_parse_double(const std::string &text, double &out)
{
    try
    {
        size_t pos;
        out = std::stod(text, &pos);
        return pos == text.size();
    }
    catch (const std::exception &)
    {
        return false;
    }
}

route_metric metro_tui::metric_from_index(int index)
{
    return index == 0 ? route_metric::DISTANCE : route_metric::TIME;
}

void metro_tui::reset_current_screen()
{
    switch (selected_tab)
    {
        case 1: // Network Info
            network_info_text.clear();
            refresh_network_info();
            break;

        case 2: // Accessibility (BFS)
            accessibility_start_index = 0;
            accessibility_target_index = 1;
            accessibility_result.clear();
            break;

        case 3: // Dijkstra
            shortest_start_index = 0;
            shortest_target_index = 1;
            shortest_metric_index = 0;
            shortest_result.clear();
            break;

        case 4: // A* vs Dijkstra
            astar_start_index = 0;
            astar_target_index = 1;
            astar_metric_index = 0;
            astar_result.clear();
            break;

        case 5: // MST
            mst_metric_index = 0;
            mst_result.clear();
            break;

        case 6: // Express Path
            express_start_index = 0;
            express_target_index = 1;
            express_result.clear();
            break;

        case 7: // Bellman-Ford
            incentive_start_index = 0;
            incentive_target_index = 1;
            incentive_result.clear();
            break;

        case 8: // Platform Scheduling
            platform_count_input.clear();
            platform_arrivals.clear();
            platform_departures.clear();
            platform_result.clear();
            if (platform_rows_container)
                platform_rows_container->DetachAllChildren();
            break;

        case 9: // Train Dispatch Queue
            dispatch_count_input.clear();
            dispatch_arrivals.clear();
            dispatch_departures.clear();
            dispatch_result.clear();
            if (dispatch_rows_container)
                dispatch_rows_container->DetachAllChildren();
            break;

        case 10: // Network Analytics
            analytics_trip_count_input.clear();
            analytics_trip_ids.clear();
            analytics_day_count_input.clear();
            analytics_k_input.clear();
            analytics_result.clear();
            if (analytics_rows_container)
                analytics_rows_container->DetachAllChildren();
            break;

        case 11: // Passenger Simulation
            passenger_capacity_input.clear();
            passenger_steps_input.clear();
            passenger_max_arrivals_input.clear();
            passenger_result.clear();
            break;

        case 12: // Floyd-Warshall
            floyd_start_index = 0;
            floyd_target_index = 1;
            floyd_metric_index = 0;
            floyd_result.clear();
            break;

        case 13: // Max Flow
            flow_source_index = 0;
            flow_target_index = 1;
            flow_route_count_input.clear();
            flow_from_indices.clear();
            flow_to_indices.clear();
            flow_capacities.clear();
            flow_current_route_index = 0;
            flow_editor_from_index = 0;
            flow_editor_to_index = 1;
            flow_editor_capacity.clear();
            flow_result.clear();
            if (flow_rows_container)
                flow_rows_container->DetachAllChildren();
            break;

        case 14: // Critical Stations
            critical_result.clear();
            break;

        case 15: // Emergency Team Placement
            emergency_result.clear();
            break;

        case 16: // Station Search
            search_query.clear();
            search_result.clear();
            break;

        default:
            break;
    }
}

void metro_tui::go_back()
{
    reset_current_screen();
    selected_tab = 0;
}

ftxui::Component metro_tui::labeled_input(
    const std::string& label,
    std::string* content,
    const std::string& placeholder)
{
    auto input = Input(content, placeholder);

    return Renderer(input, [label, input]
    {
        return hbox({
            text(label) | size(WIDTH, GREATER_THAN, 22),
            text(" "),
            input->Render() | flex,
        });
    });
}
Component metro_tui::wrap_screen(
    const std::string &title,
    Component body,
    std::string *result_text)
{
    auto back_button =
        Button(
            "Back to menu",
            [this]
            {
                go_back();
            });

    auto result_display =
        result_box(result_text, 8);

    auto layout =
        Container::Vertical({body, result_display, back_button});

    return Renderer(
        layout,
        [title, body, back_button, result_display]
        {
            return vbox({
                       text(title)
                           | bold
                           | center,

                       separator(),

                       body->Render(),

                       separator(),

                       text("Result:")
                           | bold,

                       result_display->Render(),

                       separator(),

                       back_button->Render(),
                   })
                   | border;
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

    auto screen_renderer = Renderer(menu, [this, menu]
                                    { return vbox({
                                                 text("Qom / New York Metro Routing System") | bold | center,
                                                 separator(),
                                                 text("Select a feature and press Enter:"),
                                                 separator(),
                                                 menu->Render() | frame,
                                             }) |
                                             border; });

    return drag_scroll(screen_renderer);
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
            "Refresh ",
            [this]
            {
                refresh_network_info();
            });

    return wrap_screen(
        "Network Info ",
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
    auto menu = Menu(&station_names, selected_index);

    return Renderer(menu, [label, menu]
    {
        return vbox({
            text(label) | bold,
            menu->Render()
                | vscroll_indicator
                | frame
                | size(HEIGHT, LESS_THAN, 8)
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
    catch (const std::exception &e)
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
        Container::Vertical({start_menu,
                             target_menu,
                             run_button});

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

        out << "\n Total cost:  "
            << result.total_cost;

        shortest_result =
            out.str();
    }
    catch (const std::exception &e)
    {
        shortest_result =
            std::string("Error: ") + e.what();
    }
}

Component metro_tui::build_shortest_path_screen()
{
    auto start_menu =
        create_station_menu(
            "Start station: ",
            &shortest_start_index);

    auto target_menu =
        create_station_menu(
            "Target station: ",
            &shortest_target_index);

    auto metric_selector =
        Radiobox(
            &metric_labels,
            &shortest_metric_index);

    auto run_button =
        Button(
            "Find shortest path ",
            [this]
            {
                run_shortest_path();
            });

    auto body =
        Container::Vertical({start_menu,
                             target_menu,
                             metric_selector,
                             run_button});

    return wrap_screen(
        "Shortest Path (Dijkstra) ",
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
            "Invalid station selection. ";
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

        out << "\n A* total cost: "
            << a_star_result.total_cost;

        out << "\n A* expanded nodes: "
            << a_star_result.expanded_nodes;

        out << "\n Dijkstra expanded nodes: "
            << dijkstra_result.expanded_nodes;

        astar_result =
            out.str();
    }
    catch (const std::exception &e)
    {
        astar_result =
            std::string("Error: ") + e.what();
    }
}

Component metro_tui::build_a_star_screen()
{
    auto start_menu =
        create_station_menu(
            "Start station: ",
            &astar_start_index);

    auto target_menu =
        create_station_menu(
            "Target station: ",
            &astar_target_index);

    auto metric_selector =
        Radiobox(
            &metric_labels,
            &astar_metric_index);

    auto run_button =
        Button(
            "Compare A* vs Dijkstra ",
            [this]
            {
                run_a_star_comparison();
            });

    auto body =
        Container::Vertical({start_menu,
                             target_menu,
                             metric_selector,
                             run_button});

    return wrap_screen(
        "A* vs Dijkstra ",
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

    out << " [Kruskal] Total cost:  "
        << comparison.kruskal_result.total_weight
        << " | Edges: "
        << comparison.kruskal_result.edges.size()
        << " | Time: "
        << comparison.kruskal_time_ms
        << " ms \n";

    out << "[Prim]    Total cost: "
        << comparison.prim_result.total_weight
        << " | Edges: "
        << comparison.prim_result.edges.size()
        << " | Time: "
        << comparison.prim_time_ms
        << "  ms\n";

    if (!comparison.kruskal_result.is_spanning_tree ||
        !comparison.prim_result.is_spanning_tree)
    {
        out << " Note: The graph is not connected; "
               "a complete spanning tree could not be created.\n ";
    }

    out << "\n Selected edges (Kruskal): \n";

    for (auto &e :
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

    out << "\n Selected edges (Prim): \n";

    for (auto &e :
         comparison.prim_result.edges)
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
            " Compare MST algorithms ",
            [this]
            {
                run_mst_comparison();
            });

    auto body =
        Container::Vertical({metric_selector,
                             run_button});

    return wrap_screen(
        " Minimum Cost Network (MST) ",
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
            " Invalid station selection. ";
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
                " No path exists on the express line. ";
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

        out << "\n Total time: "
            << result.total_cost
            << " minutes ";

        express_result =
            out.str();
    }
    catch (const std::logic_error &e)
    {
        express_result =
            std::string("Express line error: ") +
            e.what();
    }
    catch (const std::exception &e)
    {
        express_result =
            std::string("Error: ") + e.what();
    }
}

Component metro_tui::build_express_path_screen()
{
    auto start_menu =
        create_station_menu(
            " Start station: ",
            &express_start_index);

    auto target_menu =
        create_station_menu(
            " Target station: ",
            &express_target_index);

    auto run_button =
        Button(
            " Find express path ",
            [this]
            {
                run_express_path();
            });

    auto body =
        Container::Vertical({start_menu,
                             target_menu,
                             run_button});

    return wrap_screen(
        " Express Path (DAG) ",
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
            " Invalid station selection. ";
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
                " A negative cycle was detected "
                " in the network. ";
            return;
        }

        if (!result.reach_able)
        {
            incentive_result =
                " No path exists between these stations. ";
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

        out << "\n Final cost (including incentives): "
            << result.total_cost;

        incentive_result =
            out.str();
    }
    catch (const std::exception &e)
    {
        incentive_result =
            std::string("Error: ") + e.what();
    }
}

Component metro_tui::build_incentive_path_screen()
{
    auto start_menu =
        create_station_menu(
            " Start station: ",
            &incentive_start_index);

    auto target_menu =
        create_station_menu(
            " Target station: ",
            &incentive_target_index);

    auto run_button =
        Button(
            " Find incentive-aware path ",
            [this]
            {
                run_incentive_path();
            });

    auto body =
        Container::Vertical({start_menu,
                             target_menu,
                             run_button});

    return wrap_screen(
        " Incentive-Aware Path (Bellman-Ford) ",
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
            " Invalid number of trains . ";
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
                " Train " +
                    std::to_string(i + 1) +
                    " arrival :",
                &platform_arrivals[i],
                " time ");

        auto departure_input =
            labeled_input(
                " Train " +
                    std::to_string(i + 1) +
                    " departure: ",
                &platform_departures[i],
                " time ");

        auto row =
            Container::Horizontal({arrival_input,
                                   departure_input});

        platform_rows_container
            ->Add(row);
    }

    platform_result =
        " Rows generated. Fill them in and press Run. ";
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

    out << " Selected trains: \n";

    for (const auto &t :
         selected_trains)
    {
        out << " Train "
            << t.get_id()
            << " [ "
            << t.get_arrival_time()
            << ",  "
            << t.get_departure_time()
            << "] \n";
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
            " Number of trains: ",
            &platform_count_input,
            " count ");

    auto generate_button =
        Button(
            " Generate rows ",
            [this]
            {
                regenerate_platform_rows();
            });

    auto run_button =
        Button(
            " Run scheduling ",
            [this]
            {
                run_platform_scheduling();
            });

    auto platform_rows_view =
        make_scrollable_rows(
            platform_rows_container,
            12);

    auto body =
        Container::Vertical({count_input,
                             generate_button,
                             platform_rows_view,
                             run_button});

    return wrap_screen(
        " Platform Scheduling (Max Trains) ",
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
            " Invalid number of trains. ";
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
                " Train " +
                    std::to_string(i + 1) +
                    " arrival: ",
                &dispatch_arrivals[i],
                " time ");

        auto departure_input =
            labeled_input(
                " Train " +
                    std::to_string(i + 1) +
                    " departure: ",
                &dispatch_departures[i],
                " time ");

        auto row =
            Container::Horizontal({arrival_input,
                                   departure_input});

        dispatch_rows_container
            ->Add(row);
    }

    dispatch_result =
        " Rows generated. Fill them in and press Run. ";
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

    out << " Number of trains in queue: "
        << system.dispatch_queue_size()
        << "\n";

    if (system.dispatch_queue_empty())
    {
        out << " Dispatch queue is empty. ";
        dispatch_result =
            out.str();
        return;
    }

    train next =
        system.peek_next_train();

    out << " Highest priority train: Train  "
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

        out << " Train "
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
            " Number of trains: ",
            &dispatch_count_input,
            " count ");

    auto generate_button =
        Button(
            " Generate rows ",
            [this]
            {
                regenerate_dispatch_rows();
            });

    auto run_button =
        Button(
            " Enqueue and dispatch ",
            [this]
            {
                run_dispatch_queue();
            });

    auto dispatch_rows_view =
        make_scrollable_rows(
            dispatch_rows_container,
            12);

    auto body =
        Container::Vertical({count_input,
                             generate_button,
                             dispatch_rows_view,
                             run_button});

    return wrap_screen(
        " Train Dispatch Priority Queue ",
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
            " Invalid number of trips. ";
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
                " Trip " +
                    std::to_string(i + 1) +
                    "  station: ",
                &analytics_trip_ids[i],
                " station ID ");

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
        catch (const std::out_of_range &)
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
    catch (const std::out_of_range &)
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
            " Number of trips: ",
            &analytics_trip_count_input,
            " count ");

    auto generate_button =
        Button(
            " Generate trip rows ",
            [this]
            {
                regenerate_analytics_rows();
            });

    auto day_count_input =
        labeled_input(
            " Number of days completed: ",
            &analytics_day_count_input,
            " days ");

    auto k_input =
        labeled_input(
            " k for kth busiest station: ",
            &analytics_k_input,
            " k ");

    auto run_button =
        Button(
            " Run analytics ",
            [this]
            {
                run_network_analytics();
            });

    auto analytics_rows_view =
        make_scrollable_rows(
            analytics_rows_container,
            12);

    auto body =
        Container::Vertical({
            trip_count_input,
            generate_button,
            analytics_rows_view,
            day_count_input,
            k_input,
            run_button,
        });

    return wrap_screen(
        " Network Operations Analytics ",
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
    catch (const std::invalid_argument &e)
    {
        passenger_result =
            std::string(
                "Configuration error: ") +
            e.what();
        return;
    }

    std::ostringstream out;

    out << "Time | Queue size (after processing) | "
           " Total processed so far \n";

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
        catch (const std::invalid_argument &e)
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
        << " time units ";

    passenger_result =
        out.str();
}

Component metro_tui::build_passenger_simulation_screen()
{
    auto capacity_input =
        labeled_input(
            " Gate capacity per step: ",
            &passenger_capacity_input,
            " capacity ");

    auto steps_input =
        labeled_input(
            " Number of time steps: ",
            &passenger_steps_input,
            " steps ");

    auto max_arrivals_input =
        labeled_input(
            " Max random arrivals per step: ",
            &passenger_max_arrivals_input,
            " arrivals ");

    auto run_button =
        Button(
            " Run simulation ",
            [this]
            {
                run_passenger_simulation();
            });

    auto body =
        Container::Vertical({capacity_input,
                             steps_input,
                             max_arrivals_input,
                             run_button});

    return wrap_screen(
        " Passenger Arrival Simulation ",
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

        out << " Shortest path from "
            << system.get_station_name(start_id)
            << " to "
            << system.get_station_name(target_id)
            << ": "
            << shortest_path;

        out << (metric == route_metric::DISTANCE
                    ? " distance units"
                    : " minutes");

        floyd_result =
            out.str();
    }
    catch (const std::exception &e)
    {
        floyd_result =
            std::string("Error: ") + e.what();
    }
}

Component metro_tui::build_floyd_warshall_screen()
{
    auto start_menu =
        create_station_menu(
            " Start station: ",
            &floyd_start_index);

    auto target_menu =
        create_station_menu(
            " Target station: ",
            &floyd_target_index);

    auto metric_selector =
        Radiobox(
            &metric_labels,
            &floyd_metric_index);

    auto run_button =
        Button(
            " Find all-pairs shortest path ",
            [this]
            {
                run_floyd_warshall();
            });

    auto body =
        Container::Vertical({start_menu,
                             target_menu,
                             metric_selector,
                             run_button});

    return wrap_screen(
        " All-Pairs Shortest Path (Floyd-Warshall) ",
        body,
        &floyd_result);
}

void metro_tui::save_flow_route()
{
    if (flow_current_route_index < 0 ||
        flow_current_route_index >=
            static_cast<int>(flow_capacities.size()))
    {
        return;
    }

    flow_from_indices[flow_current_route_index] =
        flow_editor_from_index;

    flow_to_indices[flow_current_route_index] =
        flow_editor_to_index;

    flow_capacities[flow_current_route_index] =
        flow_editor_capacity;
}

void metro_tui::load_flow_route()
{
    if (flow_current_route_index < 0 ||
        flow_current_route_index >=
            static_cast<int>(flow_capacities.size()))
    {
        flow_editor_from_index = 0;
        flow_editor_to_index = 1;
        flow_editor_capacity.clear();
        return;
    }

    flow_editor_from_index =
        flow_from_indices[flow_current_route_index];

    flow_editor_to_index =
        flow_to_indices[flow_current_route_index];

    flow_editor_capacity =
        flow_capacities[flow_current_route_index];
}

void metro_tui::regenerate_flow_rows()
{
    int count;

    if (!try_parse_int(flow_route_count_input, count) ||
        count < 0)
    {
        flow_result = "Invalid route count.";
        return;
    }

    if (count == 0)
    {
        flow_from_indices.clear();
        flow_to_indices.clear();
        flow_capacities.clear();
        flow_current_route_index = 0;
        flow_editor_from_index = 0;
        flow_editor_to_index = 1;
        flow_editor_capacity.clear();
        flow_result = "Enter at least one custom route.";
        return;
    }

    flow_from_indices.assign(count, 0);
    flow_to_indices.assign(count, 0);
    flow_capacities.assign(count, "");

    flow_current_route_index = 0;
    load_flow_route();

    flow_result =
        "Routes created. Use Previous / Next to edit each route.";
}

void metro_tui::run_max_flow()
{
    save_flow_route();

    int source_id =
        get_station_id(flow_source_index);

    int target_id =
        get_station_id(flow_target_index);

    if (source_id == -1 || target_id == -1)
    {
        flow_result = "Invalid source or target station.";
        return;
    }

    if (source_id == target_id)
    {
        flow_result = "Source and target stations must be different.";
        return;
    }

    if (flow_from_indices.empty())
    {
        flow_result = "Generate at least one custom route first.";
        return;
    }

    try
    {
        for (size_t i = 0;
             i < flow_from_indices.size();
             ++i)
        {
            int from_id =
                get_station_id(flow_from_indices[i]);

            int to_id =
                get_station_id(flow_to_indices[i]);

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
                    ". Enter a numeric capacity.";
                return;
            }

            if (from_id == to_id)
            {
                flow_result =
                    "Route " +
                    std::to_string(i + 1) +
                    " cannot have the same source and target station.";
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

        std::ostringstream out;
        out << "Maximum passengers transferable: "
            << result.total_flow;

        flow_result = out.str();
    }
    catch (const std::out_of_range &)
    {
        flow_result = "Invalid station selection.";
    }
    catch (const std::invalid_argument &e)
    {
        flow_result =
            std::string("Invalid input: ") + e.what();
    }
}

Component metro_tui::build_max_flow_screen()
{
    auto source_menu =
        create_station_menu(
            "Source station",
            &flow_source_index);

    auto target_menu =
        create_station_menu(
            "Target station",
            &flow_target_index);

    auto count_input =
        labeled_input(
            "Custom routes:",
            &flow_route_count_input,
            "e.g. 3");

    auto generate_button =
        Button(
            "Generate routes",
            [this]
            {
                regenerate_flow_rows();
            });

    auto previous_button =
        Button(
            "< Previous",
            [this]
            {
                if (flow_current_route_index > 0)
                {
                    save_flow_route();
                    --flow_current_route_index;
                    load_flow_route();
                }
            });

    auto next_button =
        Button(
            "Next >",
            [this]
            {
                if (flow_current_route_index + 1 <
                    static_cast<int>(flow_capacities.size()))
                {
                    save_flow_route();
                    ++flow_current_route_index;
                    load_flow_route();
                }
            });

    auto from_menu =
        create_station_menu(
            "Route from",
            &flow_editor_from_index);

    auto to_menu =
        create_station_menu(
            "Route to",
            &flow_editor_to_index);

    auto capacity_input =
        labeled_input(
            "Capacity:",
            &flow_editor_capacity,
            "e.g. 100");

    auto run_button =
        Button(
            "Compute Max Flow",
            [this]
            {
                run_max_flow();
            });

    auto route_navigation =
        Renderer(
            Container::Horizontal({
                previous_button,
                next_button
            }),
            [this, previous_button, next_button]
            {
                std::string route_text =
                    flow_capacities.empty()
                        ? "No routes generated"
                        : "Editing route " +
                              std::to_string(flow_current_route_index + 1) +
                              " of " +
                              std::to_string(flow_capacities.size());

                return hbox({
                    previous_button->Render(),
                    filler(),
                    text(route_text) | bold | center,
                    filler(),
                    next_button->Render()
                });
            });

    auto route_editor =
        Renderer(
            Container::Vertical({
                from_menu,
                to_menu,
                capacity_input
            }),
            [this, from_menu, to_menu, capacity_input]
            {
                if (flow_capacities.empty())
                {
                    return vbox({
                        text("Generate custom routes to edit them.")
                            | color(info_color)
                            | center
                    }) | border;
                }

                return vbox({
                    text(
                        "Route " +
                        std::to_string(flow_current_route_index + 1) +
                        " configuration")
                        | bold
                        | color(section_color),

                    separator(),

                    hbox({
                        from_menu->Render() | flex,
                        separator(),
                        to_menu->Render() | flex
                    }),

                    separator(),

                    capacity_input->Render()
                }) | border;
            });

    auto setup =
        Renderer(
            Container::Vertical({
                source_menu,
                target_menu,
                count_input,
                generate_button
            }),
            [this, source_menu, target_menu, count_input, generate_button]
            {
                auto source_target =
                    hbox({
                        source_menu->Render() | flex,
                        separator(),
                        target_menu->Render() | flex
                    });

                return vbox({
                    text("NETWORK")
                        | bold
                        | color(section_color),

                    source_target,

                    separator(),

                    hbox({
                        count_input->Render() | flex,
                        text("  "),
                        generate_button->Render()
                    })
                });
            });

    auto run_area =
        Renderer(
            run_button,
            [this, run_button]
            {
                return vbox({
                    separator(),
                    run_button->Render() | center
                });
            });

    auto flow_result_display =
        result_box(&flow_result, 6);

    /*auto body =
        Container::Vertical({
            source_menu,
            target_menu,
            count_input,
            generate_button,
            previous_button,
            next_button,
            from_menu,
            to_menu,
            capacity_input,
            run_button,
            flow_result_display
        });
        */
auto body =
        Container::Vertical({
            setup,
            route_navigation,
            route_editor,
            run_area,
            flow_result_display
        });
    return Renderer(
        body,
        [this, setup, route_navigation, route_editor, run_area, flow_result_display]
        {
            return vbox({
                text("Network Capacity During Peak Hours (Max Flow)")
                    | bold
                    | color(title_color)
                    | center,

                separator(),

                setup->Render(),

                separator(),

                route_navigation->Render(),

                route_editor->Render(),

                run_area->Render(),

                separator(),

                text("Result")
                    | bold
                    | color(section_color),

                flow_result_display->Render(),

                separator(),

                text("Esc: Back to menu")
                    | color(info_color)
                    | center
            }) | border;
        });
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
        for (const auto &bridge :
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
    catch (const std::exception &e)
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
            " name ");

    auto run_button =
        Button(
            "Search",
            [this]
            {
                run_station_search();
            });

    auto body =
        Container::Vertical({query_input,
                             run_button});

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