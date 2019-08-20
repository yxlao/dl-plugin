#include <dlfcn.h>
#include <iostream>

#include "bridge.h"
#include "point.h"

int main() {
    Point a = {1, 2};
    Point b = {10, 20};

    Point c = bridge::add_point(a, b);
    std::cout << "Point c: " << c.x << ", " << c.y << std::endl;
    return 0;
}
