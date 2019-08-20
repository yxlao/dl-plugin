cmake_minimum_required (VERSION 3.0)

project (dlplugin)

add_library(point SHARED point.c)

add_executable(main main.cc)
target_link_libraries(main dl)
