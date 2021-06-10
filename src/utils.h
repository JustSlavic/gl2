#ifndef GIR1_UTIL_H
#define GIR1_UTIL_H

#include <stdio.h>
#include <string>
#include <type_traits>

#include <defines.hpp>


std::string now_as_string();
std::string read_whole_file(const char *filename);
std::string get_current_path();
bool check_gl_errors();


template <typename T>
void print_binary_repr(T x) {
    static_assert(std::is_integral<T>::value, "T must be integral");
    using uT = typename std::make_unsigned<T>::type;

    uT mask = 1;
    mask <<= (sizeof(uT)*8 - 1);

    while (mask) {
        printf("%d", (x & mask) > 0);
        mask >>= 1;
    }
    printf("\n");
}

void print_binary_repr(f32 x);
void print_binary_repr(f64 x);

#endif // GIR1_UTIL_H
