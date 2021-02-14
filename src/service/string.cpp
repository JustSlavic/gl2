#include "string.hpp"

namespace service {


int string_compare(const char* lhs, const char* rhs) {
    char a, b;
    do {
        a = *lhs++;
        b = *rhs++;
        
        // basically it's if (a == '\0' and b == '\0') but a and b are certainly equal to each other
        if (a == '\0') return a - b;
    } while (a == b);

    return a - b;
}


} // service

