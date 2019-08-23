#pragma once

// To prevent header dependencies
// Do not include this file in *.h, only in *.cc
#include "point.h"

namespace bridge {

Point add_point(Point a, Point b);
// Point add_point_three(Point a, Point b, Point c);
// Point sub_point(Point a, Point b);
// Point mul_point(Point a, Point b);

}  // namespace bridge
