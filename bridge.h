#pragma once

// Forward declares
struct Point;

namespace bridge {

struct Point add_point(struct Point a, struct Point b);

struct Point mul_point(struct Point a, struct Point b);

}  // namespace bridge
