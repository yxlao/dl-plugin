#ifdef _WIN32
#include <Windows.h>
#else
#include <dlfcn.h>
#endif
#include <functional>
#include <iostream>
#include <string>

#include "bridge.h"
#include "point.h"

// clang-format off
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
#define ARGS_12(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, ...) a12
#define ARGS_13(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, ...) a13
#define ARGS_14(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, ...) a14
#define ARGS_15(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, ...) a15
#define ARGS_16(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, ...) a16
#define ARGS_17(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, ...) a17
#define ARGS_18(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, ...) a18
#define ARGS_19(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, ...) a19
#define ARGS_20(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, ...) a20
#define ARGS_21(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, ...) a21
// clang-format on

#define COUNT_ARGS(...)                                                      \
    ARGS_21(dummy, ##__VA_ARGS__, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, \
            8, 7, 6, 5, 4, 3, 2, 1, 0)

// Convert to list of "arg_type arg_name"
// Converts:
//     float, a1, int, a2, short, a3, double, a4
// To:
//     float a1, int a2, short a3, double a4

// clang-format off
#define EXTRACT_TYPES_PARAMS_0(...)

#define EXTRACT_TYPES_PARAMS_2(...) \
    ARGS_1(__VA_ARGS__) ARGS_2(__VA_ARGS__)

#define EXTRACT_TYPES_PARAMS_4(...) \
    ARGS_1(__VA_ARGS__) ARGS_2(__VA_ARGS__), \
    ARGS_3(__VA_ARGS__) ARGS_4(__VA_ARGS__)

#define EXTRACT_TYPES_PARAMS_6(...) \
    ARGS_1(__VA_ARGS__) ARGS_2(__VA_ARGS__), \
    ARGS_3(__VA_ARGS__) ARGS_4(__VA_ARGS__), \
    ARGS_5(__VA_ARGS__) ARGS_6(__VA_ARGS__)

#define EXTRACT_TYPES_PARAMS_8(...) \
    ARGS_1 (__VA_ARGS__) ARGS_2 (__VA_ARGS__), \
    ARGS_3 (__VA_ARGS__) ARGS_4 (__VA_ARGS__), \
    ARGS_5 (__VA_ARGS__) ARGS_6 (__VA_ARGS__), \
    ARGS_7 (__VA_ARGS__) ARGS_8 (__VA_ARGS__)

#define EXTRACT_TYPES_PARAMS_10(...) \
    ARGS_1 (__VA_ARGS__) ARGS_2 (__VA_ARGS__), \
    ARGS_3 (__VA_ARGS__) ARGS_4 (__VA_ARGS__), \
    ARGS_5 (__VA_ARGS__) ARGS_6 (__VA_ARGS__), \
    ARGS_7 (__VA_ARGS__) ARGS_8 (__VA_ARGS__), \
    ARGS_9 (__VA_ARGS__) ARGS_10(__VA_ARGS__)

#define EXTRACT_TYPES_PARAMS_12(...) \
    ARGS_1 (__VA_ARGS__) ARGS_2 (__VA_ARGS__), \
    ARGS_3 (__VA_ARGS__) ARGS_4 (__VA_ARGS__), \
    ARGS_5 (__VA_ARGS__) ARGS_6 (__VA_ARGS__), \
    ARGS_7 (__VA_ARGS__) ARGS_8 (__VA_ARGS__), \
    ARGS_9 (__VA_ARGS__) ARGS_10(__VA_ARGS__), \
    ARGS_11(__VA_ARGS__) ARGS_12(__VA_ARGS__)

#define EXTRACT_TYPES_PARAMS_14(...) \
    ARGS_1 (__VA_ARGS__) ARGS_2 (__VA_ARGS__), \
    ARGS_3 (__VA_ARGS__) ARGS_4 (__VA_ARGS__), \
    ARGS_5 (__VA_ARGS__) ARGS_6 (__VA_ARGS__), \
    ARGS_7 (__VA_ARGS__) ARGS_8 (__VA_ARGS__), \
    ARGS_9 (__VA_ARGS__) ARGS_10(__VA_ARGS__), \
    ARGS_11(__VA_ARGS__) ARGS_12(__VA_ARGS__), \
    ARGS_13(__VA_ARGS__) ARGS_14(__VA_ARGS__)

#define EXTRACT_TYPES_PARAMS_16(...) \
    ARGS_1 (__VA_ARGS__) ARGS_2 (__VA_ARGS__), \
    ARGS_3 (__VA_ARGS__) ARGS_4 (__VA_ARGS__), \
    ARGS_5 (__VA_ARGS__) ARGS_6 (__VA_ARGS__), \
    ARGS_7 (__VA_ARGS__) ARGS_8 (__VA_ARGS__), \
    ARGS_9 (__VA_ARGS__) ARGS_10(__VA_ARGS__), \
    ARGS_11(__VA_ARGS__) ARGS_12(__VA_ARGS__), \
    ARGS_13(__VA_ARGS__) ARGS_14(__VA_ARGS__), \
    ARGS_15(__VA_ARGS__) ARGS_16(__VA_ARGS__)

#define EXTRACT_TYPES_PARAMS_18(...) \
    ARGS_1 (__VA_ARGS__) ARGS_2 (__VA_ARGS__), \
    ARGS_3 (__VA_ARGS__) ARGS_4 (__VA_ARGS__), \
    ARGS_5 (__VA_ARGS__) ARGS_6 (__VA_ARGS__), \
    ARGS_7 (__VA_ARGS__) ARGS_8 (__VA_ARGS__), \
    ARGS_9 (__VA_ARGS__) ARGS_10(__VA_ARGS__), \
    ARGS_11(__VA_ARGS__) ARGS_12(__VA_ARGS__), \
    ARGS_13(__VA_ARGS__) ARGS_14(__VA_ARGS__), \
    ARGS_15(__VA_ARGS__) ARGS_16(__VA_ARGS__), \
    ARGS_17(__VA_ARGS__) ARGS_18(__VA_ARGS__)

#define EXTRACT_TYPES_PARAMS_20(...) \
    ARGS_1 (__VA_ARGS__) ARGS_2 (__VA_ARGS__), \
    ARGS_3 (__VA_ARGS__) ARGS_4 (__VA_ARGS__), \
    ARGS_5 (__VA_ARGS__) ARGS_6 (__VA_ARGS__), \
    ARGS_7 (__VA_ARGS__) ARGS_8 (__VA_ARGS__), \
    ARGS_9 (__VA_ARGS__) ARGS_10(__VA_ARGS__), \
    ARGS_11(__VA_ARGS__) ARGS_12(__VA_ARGS__), \
    ARGS_13(__VA_ARGS__) ARGS_14(__VA_ARGS__), \
    ARGS_15(__VA_ARGS__) ARGS_16(__VA_ARGS__), \
    ARGS_17(__VA_ARGS__) ARGS_18(__VA_ARGS__), \
    ARGS_19(__VA_ARGS__) ARGS_20(__VA_ARGS__)
// clang-format on

// Convert to list of "arg_type arg_name" caller
#define CALL_EXTRACT_TYPES_PARAMS(num_args, ...) \
    EXTRACT_TYPES_PARAMS_##num_args(__VA_ARGS__)

// Convert to list of "arg_name"
// Converts:
//     float, a1, int, a2, short, a3, double, a4
// To:
//     a1, a2, a3, a4
// clang-format off

#define EXTRACT_PARAMS_0(...)

#define EXTRACT_PARAMS_2(...) \
    ARGS_2(__VA_ARGS__)

#define EXTRACT_PARAMS_4(...) \
    ARGS_2(__VA_ARGS__), ARGS_4(__VA_ARGS__)

#define EXTRACT_PARAMS_6(...) \
    ARGS_2(__VA_ARGS__), ARGS_4(__VA_ARGS__), \
    ARGS_6(__VA_ARGS__)

#define EXTRACT_PARAMS_8(...) \
    ARGS_2(__VA_ARGS__), ARGS_4(__VA_ARGS__), \
    ARGS_6(__VA_ARGS__), ARGS_8(__VA_ARGS__)

#define EXTRACT_PARAMS_10(...) \
    ARGS_2 (__VA_ARGS__), ARGS_4 (__VA_ARGS__), \
    ARGS_6 (__VA_ARGS__), ARGS_8 (__VA_ARGS__), \
    ARGS_10(__VA_ARGS__)

#define EXTRACT_PARAMS_12(...) \
    ARGS_2 (__VA_ARGS__), ARGS_4 (__VA_ARGS__), \
    ARGS_6 (__VA_ARGS__), ARGS_8 (__VA_ARGS__), \
    ARGS_10(__VA_ARGS__), ARGS_12(__VA_ARGS__)

#define EXTRACT_PARAMS_14(...) \
    ARGS_2 (__VA_ARGS__), ARGS_4 (__VA_ARGS__), \
    ARGS_6 (__VA_ARGS__), ARGS_8 (__VA_ARGS__), \
    ARGS_10(__VA_ARGS__), ARGS_12(__VA_ARGS__), \
    ARGS_14(__VA_ARGS__)

#define EXTRACT_PARAMS_16(...) \
    ARGS_2 (__VA_ARGS__), ARGS_4 (__VA_ARGS__), \
    ARGS_6 (__VA_ARGS__), ARGS_8 (__VA_ARGS__), \
    ARGS_10(__VA_ARGS__), ARGS_12(__VA_ARGS__), \
    ARGS_14(__VA_ARGS__), ARGS_16(__VA_ARGS__)

#define EXTRACT_PARAMS_18(...) \
    ARGS_2 (__VA_ARGS__), ARGS_4 (__VA_ARGS__), \
    ARGS_6 (__VA_ARGS__), ARGS_8 (__VA_ARGS__), \
    ARGS_10(__VA_ARGS__), ARGS_12(__VA_ARGS__), \
    ARGS_14(__VA_ARGS__), ARGS_16(__VA_ARGS__), \
    ARGS_18(__VA_ARGS__)

#define EXTRACT_PARAMS_20(...) \
    ARGS_2 (__VA_ARGS__), ARGS_4 (__VA_ARGS__), \
    ARGS_6 (__VA_ARGS__), ARGS_8 (__VA_ARGS__), \
    ARGS_10(__VA_ARGS__), ARGS_12(__VA_ARGS__), \
    ARGS_14(__VA_ARGS__), ARGS_16(__VA_ARGS__), \
    ARGS_18(__VA_ARGS__), ARGS_20(__VA_ARGS__)
// clang-format on

// Convert to list of "arg_name" caller
#define CALL_EXTRACT_PARAMS(num_args, ...) \
    EXTRACT_PARAMS_##num_args(__VA_ARGS__)

#ifdef _WIN32
HINSTANCE GetLibHandle() {
    static HINSTANCE handle = NULL;
    static const std::string lib_name = "point.dll";

    if (!handle) {
        handle = LoadLibrary(TEXT(lib_name.c_str()));
        if (handle != NULL) {
            std::cout << "Loaded " << lib_name << std::endl;
        } else {
            std::cerr << "Cannot load " << lib_name << std::endl;
            exit(1);
        }
    }

    // handle != nullptr guaranteed here
    return handle;
}
#define DEFINE_BRIDGED_FUNC_WITH_COUNT(f_name, return_type, num_args, ...) \
    return_type f_name(CALL_EXTRACT_TYPES_PARAMS(num_args, __VA_ARGS__)) { \
        typedef return_type (*f_type)(                                     \
                CALL_EXTRACT_TYPES_PARAMS(num_args, __VA_ARGS__));         \
        static f_type f = nullptr;                                         \
                                                                           \
        if (!f) {                                                          \
            f = (f_type)GetProcAddress(GetLibHandle(), #f_name);           \
            if (!f) {                                                      \
                const char* msg = dlerror();                               \
                throw std::runtime_error(std::string("Cannot load ") +     \
                                         #f_name + ": " + msg);            \
            }                                                              \
        }                                                                  \
        return f(CALL_EXTRACT_PARAMS(num_args, __VA_ARGS__));              \
    }
#else
void* GetLibHandle() {
    static void* handle = nullptr;
    static const std::string lib_name = "libpoint.so";

    if (!handle) {
        handle = dlopen(lib_name.c_str(), RTLD_LAZY);
        std::cout << "Loaded " << lib_name << std::endl;
        if (!handle) {
            const char* msg = dlerror();
            throw std::runtime_error("Cannot load " + std::string(msg));
        }
    }

    // handle != nullptr guaranteed here
    return handle;
}
#define DEFINE_BRIDGED_FUNC_WITH_COUNT(f_name, return_type, num_args, ...) \
    return_type f_name(CALL_EXTRACT_TYPES_PARAMS(num_args, __VA_ARGS__)) { \
        typedef return_type (*f_type)(                                     \
                CALL_EXTRACT_TYPES_PARAMS(num_args, __VA_ARGS__));         \
        static f_type f = nullptr;                                         \
                                                                           \
        if (!f) {                                                          \
            f = (f_type)dlsym(GetLibHandle(), #f_name);                    \
            if (!f) {                                                      \
                const char* msg = dlerror();                               \
                throw std::runtime_error(std::string("Cannot load ") +     \
                                         #f_name + ": " + msg);            \
            }                                                              \
        }                                                                  \
        return f(CALL_EXTRACT_PARAMS(num_args, __VA_ARGS__));              \
    }
#endif

#define DEFINE_BRIDGED_FUNC(f_name, return_type, ...)   \
    DEFINE_BRIDGED_FUNC_WITH_COUNT(f_name, return_type, \
                                   COUNT_ARGS(__VA_ARGS__), __VA_ARGS__)

// float add(float a, float b) { return a + b; }

// #define DEFINE_PLUGIN_FUNC_WITH_COUNT(f_name, return_type, num_args, ...)  \
//     return_type f_name(CALL_EXTRACT_TYPES_PARAMS(num_args, __VA_ARGS__)) { \
//         return add(CALL_EXTRACT_PARAMS(num_args, __VA_ARGS__));            \
//     }

// #define DEFINE_PLUGIN_FUNC(f_name, return_type, ...)   \
//     DEFINE_PLUGIN_FUNC_WITH_COUNT(f_name, return_type, \
//                                   COUNT_ARGS(__VA_ARGS__), __VA_ARGS__)

// DEFINE_PLUGIN_FUNC(foo_func, int, float, a, float, b)

namespace bridge {

// Example 1: casting to std::function
struct Point add_point(struct Point a, struct Point b) {
    static const std::string f_name = "add_point";
    using signature = struct Point(struct Point, struct Point);
    std::function<signature> f = nullptr;

    if (!f) {
        f = static_cast<signature*>(
                (signature*)GetProcAddress(GetLibHandle(), f_name.c_str()));
        if (!f) {
            throw std::runtime_error("Cannot load " + f_name);
        }
    }

    return f(a, b);
}

// Example 2: use function pointer directly
// struct Point sub_point(struct Point a, struct Point b) {
//     static const std::string f_name = "sub_point";
//     typedef struct Point (*f_type)(struct Point a, struct Point b);
//     static f_type f = nullptr;

//     if (!f) {
//         f = (f_type)dlsym(GetLibHandle(), f_name.c_str());
//         if (!f) {
//             const char* msg = dlerror();
//             throw std::runtime_error("Cannot load " + f_name + ": " +
//                                      std::string(msg));
//         }
//     }

//     return f(a, b);
// }

// Example 3: use macro
// DEFINE_BRIDGED_FUNC(add_point, Point, Point, a, Point, b)
// DEFINE_BRIDGED_FUNC(sub_point, Point, Point, a, Point, b)
// DEFINE_BRIDGED_FUNC(mul_point, Point, Point, a, Point, b)
// DEFINE_BRIDGED_FUNC(add_point_three, Point, Point, a, Point, b, Point, c)

}  // namespace bridge
