#pragma once

// https://stackoverflow.com/a/44759398/1255535
#define DEFINE_BRIDGED_FUNCTION(f_name, return_type, ...)              \
    return_type f_name(__VA_ARGS__) {                                  \
        typedef return_type (*f_type)(__VA_ARGS__);                    \
        static f_type f = nullptr;                                     \
                                                                       \
        if (!f) {                                                      \
            f = (f_type)dlsym(GetLibHandle(), #f_name);                \
            if (!f) {                                                  \
                const char* msg = dlerror();                           \
                throw std::runtime_error(std::string("Cannot load ") + \
                                         #f_name + ": " + msg);        \
            }                                                          \
        }                                                              \
        return f(__VA_ARGS__);                                         \
    }

// Forward declares
struct Point;

namespace bridge {

Point add_point(Point a, Point b);
Point sub_point(Point a, Point b);
Point mul_point(Point a, Point b);

}  // namespace bridge
