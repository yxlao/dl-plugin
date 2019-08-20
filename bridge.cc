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

// Example of casting ot std::function
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

// Example of using the function pointer directly
struct Point mul_point(struct Point a, struct Point b) {
    static const std::string func_name = "mul_point";
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

// DEFINE_BRIDGED_FUNCTION(mul_point,
//                         struct Point,
//                         struct Point(a),
//                         struct Point(b));

}  // namespace bridge
