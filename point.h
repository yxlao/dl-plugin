#ifndef POINT_H
#define POINT_H

#ifdef __cplusplus
extern "C" {
#endif

struct Point {
    int x;
    int y;
};

struct Point add_point(struct Point a, struct Point b);

struct Point add_point_three(struct Point a, struct Point b, struct Point c);

struct Point sub_point(struct Point a, struct Point b);

struct Point mul_point(struct Point a, struct Point b);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // POINT_H
