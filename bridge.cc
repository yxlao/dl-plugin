#include <dlfcn.h>
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

struct Point add_point(struct Point a, struct Point b) {
    static const std::string func_name = "add_point";
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

}  // namespace bridge
