#include <dlfcn.h>
#include <iostream>

#include "point.h"

int main() {
    Point a = {1, 2};
    Point b = {10, 20};

    void* handle = dlopen("./libpoint.so", RTLD_LAZY);
    if (!handle) {
        std::cerr << "Cannot open library: " << dlerror() << '\n';
        return 1;
    }

    std::cout << "Loading symbol hello...\n";
    dlerror();
    typedef struct Point (*add_point_t)(struct Point, struct Point);
    add_point_t add_point = (add_point_t)dlsym(handle, "add_point");
    const char* dlsym_error = dlerror();
    if (dlsym_error) {
        std::cerr << "Cannot load symbol 'hello': " << dlsym_error << '\n';
        dlclose(handle);
        return 1;
    }

    Point c = add_point(a, b);
    std::cout << "Point c: " << c.x << ", " << c.y << std::endl;
    return 0;
}
