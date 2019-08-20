#pragma once

// Forward declares
struct Point;

// struct Point add_point(struct Point a, struct Point b)
typedef struct Point (*add_point_t)(struct Point, struct Point);
add_point_t add_point;
