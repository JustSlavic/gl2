#ifndef GIR1_UTIL_H
#define GIR1_UTIL_H

#include <stdio.h>
#include <string>
#include <type_traits>

#include <defines.h>


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

void print_binary_repr(f32 x) {
    union {
        f32 f;
        u32 d;
    } f32_to_u32;
    f32_to_u32.f = x;

    u32 mask = 1;
    mask <<= (sizeof(u32)*8 - 1);

    while (mask) {
        printf("%d", (f32_to_u32.d & mask) > 0);
        mask >>= 1;
    }
    printf("\n");
}

void print_binary_repr(double x) {
    union {
        f64 f;
        u64 d;
    } f64_to_u64;
    f64_to_u64.f = x;

    u64 mask = 1;
    mask <<= (sizeof(u64)*8 - 1);

    while (mask) {
        printf("%d", (f64_to_u64.d & mask) > 0);
        mask >>= 1;
    }
    printf("\n");
}


#endif // GIR1_UTIL_H
