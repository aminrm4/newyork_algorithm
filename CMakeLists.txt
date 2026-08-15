cmake_minimum_required(VERSION 3.15)

project(qom_metro LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    ftxui
    GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui.git
    GIT_TAG v5.0.0
)

FetchContent_MakeAvailable(ftxui)

file(GLOB_RECURSE ALL_CPP
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc
)

file(GLOB_RECURSE ALL_HEADERS
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp
)

list(FILTER ALL_CPP EXCLUDE REGEX ".*/main\\.cpp$")

add_library(qom_metro_core STATIC
    ${ALL_CPP}
    ${ALL_HEADERS}
)

target_include_directories(qom_metro_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/core
    ${CMAKE_CURRENT_SOURCE_DIR}/include/algorithms
    ${CMAKE_CURRENT_SOURCE_DIR}/include/controller
    ${CMAKE_CURRENT_SOURCE_DIR}/include/ui
)

target_link_libraries(qom_metro_core PUBLIC
    ftxui::component
    ftxui::dom
)

add_executable(newyorkmetro
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)

target_link_libraries(newyorkmetro PRIVATE
    qom_metro_core
)