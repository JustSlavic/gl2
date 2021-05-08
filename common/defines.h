#ifndef GIR1_DEFINES_H
#define GIR1_DEFINES_H


#include "typedef.hpp"
#include <cstdlib>


#define STRINGIFY2(X) #X
#define STRINGIFY(X) STRINGIFY2(X)


#ifdef DEBUG
#define ASSERT(x) \
    if (!(x)) { \
        std::exit(1); \
    } void(0)

#define ASSERT_MSG(x, msg) \
    if (!(x)) { \
        printf("Assertion failed at " __FILE__ ":" STRINGIFY(__LINE__)); \
        printf("    %s\n", msg); \
        std::exit(1); \
    } void(0) \

#define GL_CALL(call) call; ASSERT(check_gl_errors())
#define GL_CHECK_ERRORS ASSERT(check_gl_errors())

#else

#define ASSERT(x)
#define ASSERT_MSG(x, msg)
#define GL_CALL(call) call
#define GL_CHECK_ERRORS

#endif

#define DEFINE_BIT_MASK_T(NAME, T, ...) \
    struct NAME { \
        enum INTERNAL : T { \
            __VA_ARGS__ \
        }; \
        T value = 0; \
        bool get(INTERNAL t) { return (value & t) != 0; } \
        void set(INTERNAL t) { value |= t; } \
        void unset(INTERNAL t) { value &= (~t); } \
    }
#define DEFINE_BIT_MASK(NAME, ...) DEFINE_BIT_MASK_T(NAME, u32, __VA_ARGS__)


#endif // GIR1_DEFINES_H
