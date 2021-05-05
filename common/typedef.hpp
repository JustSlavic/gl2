#ifndef GL2_TYPEDEF_H
#define GL2_TYPEDEF_H

#ifdef _WIN32
#define PLATFORM_WINDOWS
#endif // _WIN32
#ifdef __linux__
#define PLATFORM_LINUX
#endif // __linux__


#ifdef PLATFORM_WINDOWS

typedef __int8  i8;
typedef __int16 i16;
typedef __int16 i32;
typedef __int16 i64;

typedef unsigned __int8  u8;
typedef unsigned __int16 u16;
typedef unsigned __int16 u32;
typedef unsigned __int16 u64;

#endif // PLATFORM_WINDOWS

#ifdef PLATFORM_LINUX

typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

#endif // PLATFORM_LINUX

typedef float    f32;
typedef double   f64;

typedef float    float32;
typedef double   float64;

#endif // GL2_TYPEDEF_H
