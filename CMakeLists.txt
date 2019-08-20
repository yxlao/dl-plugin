cmake_minimum_required (VERSION 3.0)

project (dlplugin)

set(CMAKE_BUILD_TYPE Debug)

add_library(point SHARED point.c)

add_executable(main main.cc bridge.cc)
target_link_libraries(main dl)
