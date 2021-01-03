#ifndef GIR1_DEFINES_H
#define GIR1_DEFINES_H

#include <cstdio>
#include <cstdlib>
#include <cstdint>

#include <logging/logging.h>


// @todo works on my machine!
//       how do I check it compile-time on other machines?
#define ENDIANESS_LE 0
#define ENDIANESS_BE 1
#define ENDIANESS ENDIANESS_LE

#define STRINGIFY2(X) #X
#define STRINGIFY(X) STRINGIFY2(X)

#ifdef DEBUG
#define ASSERT(x) \
    if (x) { } else { \
        LOG_ERROR << "Assertion failed at " __FILE__ ":" STRINGIFY(__LINE__); \
        std::exit(1); \
    } void SEMICOLON__()

#define GL_CALL(call) call; ASSERT(check_gl_errors())
#define GL_CHECK_ERRORS ASSERT(check_gl_errors())

#else

#define ASSERT(x)
#define GL_CALL(call) call
#define GL_CHECK_ERRORS
      
#endif


typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float    f32;
typedef double   f64;

enum GIR1_ERROR {
    ERROR_GLFW_FAILED = 256,
    ERROR_GLEW_FAILED = 257,
};

#endif // GIR1_DEFINES_H
