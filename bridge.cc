#include <dlfcn.h>
#include <functional>
#include <iostream>
#include <string>

#include "bridge.h"
#include "point.h"

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#define ARGS_1(a1, ...) a1
#define ARGS_2(a1, a2, ...) a2
#define ARGS_3(a1, a2, a3, ...) a3
#define ARGS_4(a1, a2, a3, a4, ...) a4
#define ARGS_5(a1, a2, a3, a4, a5, ...) a5
#define ARGS_6(a1, a2, a3, a4, a5, a6, ...) a6
#define ARGS_7(a1, a2, a3, a4, a5, a6, a7, ...) a7
#define ARGS_8(a1, a2, a3, a4, a5, a6, a7, a8, ...) a8
#define ARGS_9(a1, a2, a3, a4, a5, a6, a7, a8, a9, ...) a9
#define ARGS_10(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, ...) a10
#define ARGS_11(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, ...) a11

#define COUNT_ARGS(...) \
    ARGS_11(dummy, ##__VA_ARGS__, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

// TYPES, PARAMS
#define EXTRACT_TYPES_PARAMS_4(...) \
    ARGS_1(__VA_ARGS__)             \
    ARGS_2(__VA_ARGS__), ARGS_3(__VA_ARGS__) ARGS_4(__VA_ARGS__)

#define CALL_EXTRACT_TYPES_PARAMS(num_args, ...) \
    EXTRACT_TYPES_PARAMS_##num_args(__VA_ARGS__)

#define COUNT_CALL_EXTRACT_TYPES_PARAMS(...) \
    CALL_EXTRACT_TYPES_PARAMS(COUNT_ARGS(__VA_ARGS__), __VA_ARGS__)

// PARAMS
#define EXTRACT_PARAMS_4(...) ARGS_2(__VA_ARGS__), ARGS_4(__VA_ARGS__)

#define CALL_EXTRACT_PARAMS(num_args, ...) \
    EXTRACT_PARAMS_##num_args(__VA_ARGS__)

#define COUNT_CALL_EXTRACT_PARAMS(...) \
    CALL_EXTRACT_PARAMS(COUNT_ARGS(__VA_ARGS__), __VA_ARGS__)

int ARGS_2(float, a, float, b) = 100;
int ARGS_4(float, a, float, b) = 200;
int EXTRACT_PARAMS_4(float, c, float, d) = 4000;
int four = COUNT_ARGS(float, e, float, f);
int four2 = COUNT_ARGS(float, i, float, j);
int CALL_EXTRACT_PARAMS(4, float, g, float, h) = 5000;
std::string s = TOSTRING(COUNT_ARGS(float, e, float, f));
float add(float a, float b) { return a + b; }

#define DEFINE_PLUGIN_FUNC(f_name, return_type, num_args, ...)             \
    return_type f_name(CALL_EXTRACT_TYPES_PARAMS(num_args, __VA_ARGS__)) { \
        return add(CALL_EXTRACT_PARAMS(num_args, __VA_ARGS__));            \
    }

    // DEFINE_PLUGIN_FUNC(foo_func, int, 4, float, a, float, b)

#define DEFINE_PLUGIN_FUNC_AUTO_COUNT(f_name, return_type, ...)      \
    DEFINE_PLUGIN_FUNC(f_name, return_type, COUNT_ARGS(__VA_ARGS__), \
                       __VA_ARGS__)
DEFINE_PLUGIN_FUNC_AUTO_COUNT(foo_func, int, float, a, float, b)

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

void* GetLibHandle() {
    static void* handle = nullptr;
    static const std::string lib_name = "libpoint.so";

    if (!handle) {
        handle = dlopen(lib_name.c_str(), RTLD_LAZY);
        std::cout << "Loaded " << lib_name << std::endl;
        std::cout << "a " << a << std::endl;
        std::cout << "b " << b << std::endl;
        std::cout << "four " << four << std::endl;
        std::cout << "c " << c << std::endl;
        std::cout << "d " << d << std::endl;
        std::cout << "h " << h << std::endl;
        std::cout << "s " << s << std::endl;
        std::cout << "foo_func(3, 5) " << foo_func(3, 5) << std::endl;
        if (!handle) {
            const char* msg = dlerror();
            throw std::runtime_error("Cannot load " + std::string(msg));
        }
    }

    // handle != nullptr guaranteed here
    return handle;
}

namespace bridge {

// Example 1: casting to std::function
struct Point add_point(struct Point a, struct Point b) {
    static const std::string f_name = "add_point";
    using signature = struct Point(struct Point, struct Point);
    std::function<signature> f = nullptr;

    if (!f) {
        f = static_cast<signature*>(
                (signature*)dlsym(GetLibHandle(), f_name.c_str()));
        if (!f) {
            const char* msg = dlerror();
            throw std::runtime_error("Cannot load " + f_name + ": " +
                                     std::string(msg));
        }
    }

    return f(a, b);
}

// Example 2: use function pointer directly
struct Point sub_point(struct Point a, struct Point b) {
    static const std::string f_name = "sub_point";
    typedef struct Point (*f_type)(struct Point a, struct Point b);
    static f_type f = nullptr;

    if (!f) {
        f = (f_type)dlsym(GetLibHandle(), f_name.c_str());
        if (!f) {
            const char* msg = dlerror();
            throw std::runtime_error("Cannot load " + f_name + ": " +
                                     std::string(msg));
        }
    }

    return f(a, b);
}

// Example 3: use macro
DEFINE_BRIDGED_FUNCTION(mul_point, Point, Point(a), Point(b))
// Point mul_point(Point(a), Point(b)) {
//     static const std::string f_name = "mul_point";
//     typedef Point (*f_type)(Point(a), Point(b));
//     static f_type f = nullptr;
//     if (!f) {
//         f = (f_type)dlsym(GetLibHandle(), "mul_point");
//         if (!f) {
//             const char* msg = dlerror();
//             throw std::runtime_error(std::string("Cannot load ") +
//                                      "mul_point" + ": " + msg);
//         }
//     }
//     return f(Point(a), Point(b));
// }

}  // namespace bridge
