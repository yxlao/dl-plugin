#include "point.h"

struct Point add_point(struct Point a, struct Point b) {
    struct Point c;
    c.x = a.x + b.x;
    c.y = a.y + b.y;
    return c;
}

struct Point add_point_three(struct Point a, struct Point b, struct Point c) {
    struct Point d;
    d.x = a.x + b.x + c.x;
    d.y = a.y + b.y + c.y;
    return d;
}

struct Point sub_point(struct Point a, struct Point b) {
    struct Point c;
    c.x = a.x - b.x;
    c.y = a.y - b.y;
    return c;
}

struct Point mul_point(struct Point a, struct Point b) {
    struct Point c;
    c.x = a.x * b.x;
    c.y = a.y * b.y;
    return c;
}
