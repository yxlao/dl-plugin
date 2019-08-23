#include <iostream>

#include "bridge.h"
#include "point.h"

int main() {
    Point a = {1, 1};
    Point b = {2, 2};
    Point c = {3, 3};

    std::cout << "hello" << std::endl;
    Point add = bridge::add_point(a, b);
    std::cout << "add: " << std::endl;
    std::cout << "add: " << add.x << ", " << add.y << std::endl;

    Point add_three = bridge::add_point_three(a, b, c);
    std::cout << "add_three: " << add_three.x << ", " << add_three.y
              << std::endl;

    Point sub = bridge::sub_point(a, b);
    std::cout << "sub: " << sub.x << ", " << sub.y << std::endl;

    Point mul = bridge::mul_point(a, b);
    std::cout << "mul: " << mul.x << ", " << mul.y << std::endl;

    return 0;
}
