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

#define EXPAND(x) x  // MSVC fix

// Count __VA_ARGS__ for MSVC https://stackoverflow.com/a/26685339/1255535
#ifdef _MSC_VER

#define __NARGS(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, \
                _15, _16, _17, _18, _19, _20, _21, VAL, ...)                 \
    VAL
#define NARGS_1(...)                                                           \
    EXPAND(__NARGS(__VA_ARGS__, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, \
                   8, 7, 6, 5, 4, 3, 2, 1, 0))
#define AUGMENTER(...) unused, __VA_ARGS__
#define COUNT_ARGS(...) NARGS_1(AUGMENTER(__VA_ARGS__))

#else
#define COUNT_ARGS(...)                                                       \
    __NARGS(0, ##__VA_ARGS__, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, \
            9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define __NARGS(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, \
                _14, _15, _16, _17, _18, _19, _20, _21, N, ...)             \
    N

#endif

// Convert to list of "arg_type arg_name"
// Converts:
//     float, a1, int, a2, short, a3, double, a4
// To:
//     float a1, int a2, short a3, double a4

// clang-format off
#define EXTRACT_TYPES_PARAMS_0(...)

#define EXTRACT_TYPES_PARAMS_2(...) \
    EXPAND(ARGS_1(__VA_ARGS__)) EXPAND(ARGS_2(__VA_ARGS__))

#define EXTRACT_TYPES_PARAMS_4(...) \
    EXPAND(ARGS_1(__VA_ARGS__)) EXPAND(ARGS_2(__VA_ARGS__)), \
    EXPAND(ARGS_3(__VA_ARGS__)) EXPAND(ARGS_4(__VA_ARGS__))

#define EXTRACT_TYPES_PARAMS_6(...) \
    EXPAND(ARGS_1(__VA_ARGS__)) EXPAND(ARGS_2(__VA_ARGS__)), \
    EXPAND(ARGS_3(__VA_ARGS__)) EXPAND(ARGS_4(__VA_ARGS__)), \
    EXPAND(ARGS_5(__VA_ARGS__)) EXPAND(ARGS_6(__VA_ARGS__))

#define EXTRACT_TYPES_PARAMS_8(...) \
    EXPAND(ARGS_1 (__VA_ARGS__)) EXPAND(ARGS_2 (__VA_ARGS__)), \
    EXPAND(ARGS_3 (__VA_ARGS__)) EXPAND(ARGS_4 (__VA_ARGS__)), \
    EXPAND(ARGS_5 (__VA_ARGS__)) EXPAND(ARGS_6 (__VA_ARGS__)), \
    EXPAND(ARGS_7 (__VA_ARGS__)) EXPAND(ARGS_8 (__VA_ARGS__))

#define EXTRACT_TYPES_PARAMS_10(...) \
    EXPAND(ARGS_1 (__VA_ARGS__)) EXPAND(ARGS_2 (__VA_ARGS__)), \
    EXPAND(ARGS_3 (__VA_ARGS__)) EXPAND(ARGS_4 (__VA_ARGS__)), \
    EXPAND(ARGS_5 (__VA_ARGS__)) EXPAND(ARGS_6 (__VA_ARGS__)), \
    EXPAND(ARGS_7 (__VA_ARGS__)) EXPAND(ARGS_8 (__VA_ARGS__)), \
    EXPAND(ARGS_9 (__VA_ARGS__)) EXPAND(ARGS_10(__VA_ARGS__))

#define EXTRACT_TYPES_PARAMS_12(...) \
    EXPAND(ARGS_1 (__VA_ARGS__)) EXPAND(ARGS_2 (__VA_ARGS__)), \
    EXPAND(ARGS_3 (__VA_ARGS__)) EXPAND(ARGS_4 (__VA_ARGS__)), \
    EXPAND(ARGS_5 (__VA_ARGS__)) EXPAND(ARGS_6 (__VA_ARGS__)), \
    EXPAND(ARGS_7 (__VA_ARGS__)) EXPAND(ARGS_8 (__VA_ARGS__)), \
    EXPAND(ARGS_9 (__VA_ARGS__)) EXPAND(ARGS_10(__VA_ARGS__)), \
    EXPAND(ARGS_11(__VA_ARGS__)) EXPAND(ARGS_12(__VA_ARGS__))

#define EXTRACT_TYPES_PARAMS_14(...) \
    EXPAND(ARGS_1 (__VA_ARGS__)) EXPAND(ARGS_2 (__VA_ARGS__)), \
    EXPAND(ARGS_3 (__VA_ARGS__)) EXPAND(ARGS_4 (__VA_ARGS__)), \
    EXPAND(ARGS_5 (__VA_ARGS__)) EXPAND(ARGS_6 (__VA_ARGS__)), \
    EXPAND(ARGS_7 (__VA_ARGS__)) EXPAND(ARGS_8 (__VA_ARGS__)), \
    EXPAND(ARGS_9 (__VA_ARGS__)) EXPAND(ARGS_10(__VA_ARGS__)), \
    EXPAND(ARGS_11(__VA_ARGS__)) EXPAND(ARGS_12(__VA_ARGS__)), \
    EXPAND(ARGS_13(__VA_ARGS__)) EXPAND(ARGS_14(__VA_ARGS__))

#define EXTRACT_TYPES_PARAMS_16(...) \
    EXPAND(ARGS_1 (__VA_ARGS__)) EXPAND(ARGS_2 (__VA_ARGS__)), \
    EXPAND(ARGS_3 (__VA_ARGS__)) EXPAND(ARGS_4 (__VA_ARGS__)), \
    EXPAND(ARGS_5 (__VA_ARGS__)) EXPAND(ARGS_6 (__VA_ARGS__)), \
    EXPAND(ARGS_7 (__VA_ARGS__)) EXPAND(ARGS_8 (__VA_ARGS__)), \
    EXPAND(ARGS_9 (__VA_ARGS__)) EXPAND(ARGS_10(__VA_ARGS__)), \
    EXPAND(ARGS_11(__VA_ARGS__)) EXPAND(ARGS_12(__VA_ARGS__)), \
    EXPAND(ARGS_13(__VA_ARGS__)) EXPAND(ARGS_14(__VA_ARGS__)), \
    EXPAND(ARGS_15(__VA_ARGS__)) EXPAND(ARGS_16(__VA_ARGS__))

#define EXTRACT_TYPES_PARAMS_18(...) \
    EXPAND(ARGS_1 (__VA_ARGS__)) EXPAND(ARGS_2 (__VA_ARGS__)), \
    EXPAND(ARGS_3 (__VA_ARGS__)) EXPAND(ARGS_4 (__VA_ARGS__)), \
    EXPAND(ARGS_5 (__VA_ARGS__)) EXPAND(ARGS_6 (__VA_ARGS__)), \
    EXPAND(ARGS_7 (__VA_ARGS__)) EXPAND(ARGS_8 (__VA_ARGS__)), \
    EXPAND(ARGS_9 (__VA_ARGS__)) EXPAND(ARGS_10(__VA_ARGS__)), \
    EXPAND(ARGS_11(__VA_ARGS__)) EXPAND(ARGS_12(__VA_ARGS__)), \
    EXPAND(ARGS_13(__VA_ARGS__)) EXPAND(ARGS_14(__VA_ARGS__)), \
    EXPAND(ARGS_15(__VA_ARGS__)) EXPAND(ARGS_16(__VA_ARGS__)), \
    EXPAND(ARGS_17(__VA_ARGS__)) EXPAND(ARGS_18(__VA_ARGS__))

#define EXTRACT_TYPES_PARAMS_20(...) \
    EXPAND(ARGS_1 (__VA_ARGS__)) EXPAND(ARGS_2 (__VA_ARGS__)), \
    EXPAND(ARGS_3 (__VA_ARGS__)) EXPAND(ARGS_4 (__VA_ARGS__)), \
    EXPAND(ARGS_5 (__VA_ARGS__)) EXPAND(ARGS_6 (__VA_ARGS__)), \
    EXPAND(ARGS_7 (__VA_ARGS__)) EXPAND(ARGS_8 (__VA_ARGS__)), \
    EXPAND(ARGS_9 (__VA_ARGS__)) EXPAND(ARGS_10(__VA_ARGS__)), \
    EXPAND(ARGS_11(__VA_ARGS__)) EXPAND(ARGS_12(__VA_ARGS__)), \
    EXPAND(ARGS_13(__VA_ARGS__)) EXPAND(ARGS_14(__VA_ARGS__)), \
    EXPAND(ARGS_15(__VA_ARGS__)) EXPAND(ARGS_16(__VA_ARGS__)), \
    EXPAND(ARGS_17(__VA_ARGS__)) EXPAND(ARGS_18(__VA_ARGS__)), \
    EXPAND(ARGS_19(__VA_ARGS__)) EXPAND(ARGS_20(__VA_ARGS__))
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
    EXPAND(ARGS_2(__VA_ARGS__))

#define EXTRACT_PARAMS_4(...) \
    EXPAND(ARGS_2(__VA_ARGS__)), EXPAND(ARGS_4(__VA_ARGS__))

#define EXTRACT_PARAMS_6(...) \
    EXPAND(ARGS_2(__VA_ARGS__)), EXPAND(ARGS_4(__VA_ARGS__)), \
    EXPAND(ARGS_6(__VA_ARGS__))

#define EXTRACT_PARAMS_8(...) \
    EXPAND(ARGS_2(__VA_ARGS__)), EXPAND(ARGS_4(__VA_ARGS__)), \
    EXPAND(ARGS_6(__VA_ARGS__)), EXPAND(ARGS_8(__VA_ARGS__))

#define EXTRACT_PARAMS_10(...) \
    EXPAND(ARGS_2 (__VA_ARGS__)), EXPAND(ARGS_4 (__VA_ARGS__)), \
    EXPAND(ARGS_6 (__VA_ARGS__)), EXPAND(ARGS_8 (__VA_ARGS__)), \
    EXPAND(ARGS_10(__VA_ARGS__))

#define EXTRACT_PARAMS_12(...) \
    EXPAND(ARGS_2 (__VA_ARGS__)), EXPAND(ARGS_4 (__VA_ARGS__)), \
    EXPAND(ARGS_6 (__VA_ARGS__)), EXPAND(ARGS_8 (__VA_ARGS__)), \
    EXPAND(ARGS_10(__VA_ARGS__)), EXPAND(ARGS_12(__VA_ARGS__))

#define EXTRACT_PARAMS_14(...) \
    EXPAND(ARGS_2 (__VA_ARGS__)), EXPAND(ARGS_4 (__VA_ARGS__)), \
    EXPAND(ARGS_6 (__VA_ARGS__)), EXPAND(ARGS_8 (__VA_ARGS__)), \
    EXPAND(ARGS_10(__VA_ARGS__)), EXPAND(ARGS_12(__VA_ARGS__)), \
    EXPAND(ARGS_14(__VA_ARGS__))

#define EXTRACT_PARAMS_16(...) \
    EXPAND(ARGS_2 (__VA_ARGS__)), EXPAND(ARGS_4 (__VA_ARGS__)), \
    EXPAND(ARGS_6 (__VA_ARGS__)), EXPAND(ARGS_8 (__VA_ARGS__)), \
    EXPAND(ARGS_10(__VA_ARGS__)), EXPAND(ARGS_12(__VA_ARGS__)), \
    EXPAND(ARGS_14(__VA_ARGS__)), EXPAND(ARGS_16(__VA_ARGS__))

#define EXTRACT_PARAMS_18(...) \
    EXPAND(ARGS_2 (__VA_ARGS__)), EXPAND(ARGS_4 (__VA_ARGS__)), \
    EXPAND(ARGS_6 (__VA_ARGS__)), EXPAND(ARGS_8 (__VA_ARGS__)), \
    EXPAND(ARGS_10(__VA_ARGS__)), EXPAND(ARGS_12(__VA_ARGS__)), \
    EXPAND(ARGS_14(__VA_ARGS__)), EXPAND(ARGS_16(__VA_ARGS__)), \
    EXPAND(ARGS_18(__VA_ARGS__))

#define EXTRACT_PARAMS_20(...) \
    EXPAND(ARGS_2 (__VA_ARGS__)), EXPAND(ARGS_4 (__VA_ARGS__)), \
    EXPAND(ARGS_6 (__VA_ARGS__)), EXPAND(ARGS_8 (__VA_ARGS__)), \
    EXPAND(ARGS_10(__VA_ARGS__)), EXPAND(ARGS_12(__VA_ARGS__)), \
    EXPAND(ARGS_14(__VA_ARGS__)), EXPAND(ARGS_16(__VA_ARGS__)), \
    EXPAND(ARGS_18(__VA_ARGS__)), EXPAND(ARGS_20(__VA_ARGS__))
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
        static f_type f = NULL;                                            \
                                                                           \
        if (f == NULL) {                                                   \
            f = (f_type)GetProcAddress(GetLibHandle(), #f_name);           \
            if (f != NULL) {                                               \
                std::cout << "Loaded func " << #f_name << std::endl;       \
            } else {                                                       \
                std::cerr << "Cannot load func " << #f_name << std::endl;  \
                exit(1);                                                   \
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

// Example: use function pointer directly
// Point add_point(Point a, Point b) {
//     // int count = COUNT_ARGS(Point, a, Point, b);
//     CALL_EXTRACT_TYPES_PARAMS(2, float, count) = 30;
//     std::cout << "count: " << count << std::endl;

//     typedef Point (*f_type)(Point a, Point b);
//     static f_type f = 0;
//     if (f == 0) {
//         f = (f_type)GetProcAddress(GetLibHandle(), "add_point");
//         if (f != 0) {
//             std::cout << "Loaded func "
//                       << "add_point" << std::endl;
//         } else {
//             std::cerr << "Cannot load func "
//                       << "add_point" << std::endl;
//             exit(1);
//         }
//     }
//     return f(a, b);
// }

// Example 3: use macro
DEFINE_BRIDGED_FUNC(add_point, Point, Point, a, Point, b)
DEFINE_BRIDGED_FUNC(sub_point, Point, Point, a, Point, b)
DEFINE_BRIDGED_FUNC(mul_point, Point, Point, a, Point, b)
DEFINE_BRIDGED_FUNC(add_point_three, Point, Point, a, Point, b, Point, c)

}  // namespace bridge
