#include <dlfcn.h>
#include <functional>
#include <iostream>
#include <string>

#include "bridge.h"
#include "point.h"

void* GetLibHandle() {
    static void* handle = nullptr;
    static const std::string lib_name = "libpoint.so";

    if (!handle) {
        handle = dlopen(lib_name.c_str(), RTLD_LAZY);
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
    static const std::string func_name = "add_point";
    using signature = struct Point(struct Point, struct Point);
    std::function<signature> func = nullptr;

    if (!func) {
        func = static_cast<signature*>(
                (signature*)dlsym(GetLibHandle(), func_name.c_str()));
        if (!func) {
            const char* msg = dlerror();
            throw std::runtime_error("Cannot load " + func_name + ": " +
                                     std::string(msg));
        }
    }

    return func(a, b);
}

// Example 2: use function pointer directly
struct Point sub_point(struct Point a, struct Point b) {
    static const std::string func_name = "sub_point";
    typedef struct Point (*func_t)(struct Point a, struct Point b);
    static func_t func = nullptr;

    if (!func) {
        func = (func_t)dlsym(GetLibHandle(), func_name.c_str());
        if (!func) {
            const char* msg = dlerror();
            throw std::runtime_error("Cannot load " + func_name + ": " +
                                     std::string(msg));
        }
    }

    return func(a, b);
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
