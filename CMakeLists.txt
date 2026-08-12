cmake_minimum_required(VERSION 3.15)
project(qom_metro LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- core graph module
set(CORE_SOURCES
    src/core/station.cpp
    src/core/edge.cpp
    src/core/graph.cpp
    src/core/qom_network_data.cpp
    src/core/train.cc
    src/core/passenger.cpp
)

# --- pathfinding algorithms 
set(ALGORITHM_SOURCES
    src/algorithms/bfs_algorithm.cpp
    src/algorithms/dfs_algorithm.cpp
    src/algorithms/dijkstra_algorithm.cpp
    src/algorithms/dispatch_queue.cpp
    src/algorithms/min_heap.cpp
    src/algorithms/network_analytics.cpp
    src/algorithms/passenger_simulator.cpp
    src/algorithms/platform_scheduler.cc




)


add_library(qom_metro_core STATIC
    ${CORE_SOURCES}
    ${ALGORITHM_SOURCES}
)

target_include_directories(qom_metro_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/core
    ${CMAKE_CURRENT_SOURCE_DIR}/include/algorithms
)


add_executable(qom_metro src/main.cpp)
target_link_libraries(qom_metro PRIVATE qom_metro_core)

