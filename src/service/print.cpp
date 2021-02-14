#include "print.hpp"

#include <cstdio>

namespace service {

int print (const char* pattern, ...) {
    printf(pattern, ...);

    return 0;
}

} // service
