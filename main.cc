#include <dlfcn.h>
#include <iostream>

#include "bridge.h"
#include "point.h"

int main() {
    Point a = {1, 2};
    Point b = {10, 20};

    Point c = bridge::add_point(a, b);
    std::cout << "Point c: " << c.x << ", " << c.y << std::endl;

    Point d = bridge::sub_point(a, b);
    std::cout << "Point d: " << d.x << ", " << d.y << std::endl;

    Point e = bridge::mul_point(a, b);
    std::cout << "Point d: " << e.x << ", " << e.y << std::endl;
    return 0;
}
