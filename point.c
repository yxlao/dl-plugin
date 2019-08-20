#include "point.h"

struct Point add_point(struct Point a, struct Point b) {
    struct Point c;
    c.x = a.x + b.x;
    c.y = a.y + b.y;
    return c;
}
